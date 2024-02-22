#include <Adafruit_ADS1X15.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 //Szerokość ekranu
#define SCREEN_HEIGHT 32 //Wysokość ekranu
#define OLED_RESET     -1 //Resetowanie Ekranu 
#define SCREEN_ADDRESS 0x3C //Adres Ekranu
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Definiowanie ekranu

Adafruit_ADS1115 ads; //Definiowanie ads

unsigned int rectPossition; //Pozycja prostokąta z zadaną siłą
unsigned int rectSize = 12; //Rozmiar kwadratu zależny od zmiennej difficulty
unsigned long score = 0; //Deklaracja zmiennej wyniku
unsigned long currentTimeOnSpot = 0; //Zmienna do mierzenia czasu przez jaki wskaźnik znajduje się w prostokącie
unsigned int lastScore = 0; //Zmienna do przechowywania ostatniego wyniku
double meanScore = 0; //Zmienna do średniej z wyników
unsigned int sesionCounter = 0; //Zmienna do liczenia ilości sesji

unsigned long remTime = 0; //Deklaracja zmiennej zapamiętanego czasu
unsigned long difTime = 0; //Deklaracja zmiennej różnicy czasu
unsigned long estTime = 10000; //Deklaracja zmiennej czasu trwania ćwiczenia
unsigned long remClockTime = 0; //Deklaracja czasu zapamiętanego odliczania 
unsigned long clockTime = 0; // Deklaracja zmiennej czasu odliczania (również czasu trwania sesji)

int button1pin = D3; //Deklaracja pinów pod które podpięty jest przycisk
int button2pin = D2; //
int button1val;      //Deklaracja zmiennych do przechowywania wartości przycisków
int button2val;      //
int button1prevVal;  //Deklaracja zmiennych do przechowywania poprzednich wartości przycisków
int button2prevVal;  //   
int option = 0;      //Zmienna do wybierania opcji w menu
int a = 1;           //Zmienna do działania pętli
int rectPossitionChange = 0; //Zmienna do zmiany pozycji prostokąta
const char *menuItems[] = {"Cwiczenie", "Wyniki", "Opcje"};  //Przechowywanie wartości menu 

int chosedOption = 2; //Deklaracja zmiennej wyboru w menu opcji
int difficulty = 1;   //Zmienna do określania poziomu trudności


void options()  //Funkcja menu do zmianny opcji
{
  delay(200);
  while(a == 1){
    display.clearDisplay();
    display.drawRect(0, chosedOption, 128, 14, 1);
    display.setTextSize(1);
    display.setCursor(6, 5);
    display.print("Czas cwiczenia:");
    display.setCursor(6, 18);
    display.print("Poziom trudnosci:");
    display.setCursor(111, 5);
    display.print(estTime/1000);
    display.setCursor(111, 18);
    display.print(difficulty); 
    display.display();

    button1val = digitalRead(button1pin);
    button2val = digitalRead(button2pin);

    if(button1val == LOW && button1prevVal == HIGH && chosedOption == 15){
      chosedOption = 2;
      break;
    }

    if(button1val == LOW && button1prevVal == HIGH && chosedOption == 2){
      chosedOption = 15;
    }

    if(button2val == LOW && button2prevVal == HIGH && chosedOption == 2){
      estTime = estTime + 1000;
      if(estTime > 60000){
        estTime = 10000;
      }
      delay(200); 
    }
    if(button2val == LOW && button2prevVal == HIGH && chosedOption == 15){
      difficulty += 1;
      if(difficulty > 3){
        difficulty = 1;
      }
      delay(200);
    }

    button1prevVal = button1val;  
    button2prevVal = button2val;
  }
}

void results() //Funkcja menu wynikow
{
  delay(200);
  while(a == 1){
    button1val = digitalRead(button1pin);
    button2val = digitalRead(button2pin);
    display.clearDisplay();
    display.drawRect(0, 0, 128, 32, 1);
    display.setTextColor(1);
    display.setTextSize(1);
    display.setCursor(5, 5);
    display.print("Ostatni wynik:");
    display.setCursor(5, 17);
    display.print("Srednia:");
    display.setCursor(93, 5);
    display.print(lastScore);
    display.setCursor(93, 17);
    display.print(meanScore);
    display.display();
    //Wyjscie z menu
    if(button1val == LOW && button1prevVal == HIGH){  
      break;
    }
    //Kasowanie wyników
    if(button2val == LOW && button2prevVal == HIGH){
      sesionCounter = 0;
      lastScore = 0;
      meanScore = 0;
    }
    button1prevVal = button1val;
    button2prevVal = button2val;
    delay(200);
  }
}

void endSesion() //Funkcja końca sesji ćwiczeń
{
    display.clearDisplay();
    
    display.setTextColor(1);
    display.setTextSize(1);
    display.setCursor(10, 12);
    display.setTextWrap(false);               //Wyświetlanie wyniku po zakończonej sesji
    display.print("Twoj wynik to: ");
    display.print(score/1000);
    display.display();
    
    sesionCounter ++;
    lastScore = score/1000;
    meanScore = ((meanScore * (sesionCounter - 1)) + score/1000)/sesionCounter;
    score = 0;
    
    delay(3000);
    
}

void exercise() //Funkcja z kodem ćwiczenia
{
  score = 0;
  if(difficulty == 1){
    rectSize = 12;
    rectPossitionChange = 0;
  }
  if(difficulty == 2){                 //Rozmiar prostokąta zależny od zmiennej difficulty
    rectSize = 10;
    rectPossitionChange = 0;
  }
  if(difficulty == 3){
    rectSize = 8;
    rectPossitionChange = 1;
  }
  rectPossition = random(81); //Losowanie pozycji prostokąta z założoną siłą
  remClockTime = millis();    
  while(a == 1){
    int16_t adc0;
    float volts0;
    int16_t progress; //Zmienna w której zawarta jest aktualna siła nacisku

    difTime = millis() - remTime; //Obliczanie różnicy czasu
    adc0 = ads.readADC_SingleEnded(0); //Odczytywanie informacji z ADS1115
    volts0 = ads.computeVolts(adc0);
    if (volts0 < 0.44){
      progress = 0;
    }
    else if (volts0 >= 0.44 && volts0 <= 3.81){
      progress = 3.24*exp(1.37*volts0);
    }
    else{
      progress = 0.00005727*exp(4.25*volts0); //Przeliczenie wartości z ADC na rzeczywistą masę
    }

    clockTime = millis() - remClockTime; //Czas trwania ćwiczenia (odejmujemy wartość zapamiętaną podczas ostatniej sesji)

    if(rectPossitionChange == 1 && clockTime > estTime/2){
      rectPossition = random(81);                        //Jeśli poziom trudności == 3, to dodajemy dodatkowe utrudnienie w postaci zmiany pozycji prostokąta w połowie sesji
      rectPossitionChange = 0;
    }

    display.drawRect(rectPossition, 0, rectSize, 32, 1); //Rysowanie prostokąta z założoną siłą
    display.fillRect(0, 10, round(progress/10), 12, 1); //Rysowanie paska przedstawiającego aktualną siłę nacisku 

    //Wyświetlanie wyniku na ekranie
    display.setTextColor(1);
    display.setTextSize(1);
    display.setCursor(103, 12);
    display.print(score/1000); 

    //Wyświetlanie aktualnej masy
    display.setCursor(95, 2);
    display.print(progress); 

    //Wyświetlanie czasu na ekranie
    display.setCursor(102, 22);
    display.print((clockTime)/1000);

    display.display();

    
    if(progress/10 > rectPossition && progress/10 < rectPossition + rectSize){
      score += (millis() - currentTimeOnSpot); //Warunek obliczający wynik                                                 
    }
    currentTimeOnSpot = millis(); 
    
    if (difTime >= 100){
        remTime = millis();         
    }

    if(clockTime > estTime){
      endSesion();                  //Warunek końca sesji
      break;
    }

    display.clearDisplay();  //Czyszczenie ekranu
  }
}

void setup(void)
{

  Serial.begin(9600); 

  pinMode(button1pin, INPUT_PULLUP); //Deklaracja pinów na których są przyciski z pullup'em 
  pinMode(button2pin, INPUT_PULLUP);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS."); //Warunek, który w przypadku problemów z inicjacją ADS wyświetla informację
    while (1);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));          //Warunek, który w przypadku błędu związanego z ekranem wyświetla informację 
    for(;;);
  }

  display.display();      //Próba działania ekranu
  display.clearDisplay(); //

}

void loop(void)  //Główna pętla programu, w której mamy menu głowne
{
  display.clearDisplay();
  display.setTextColor(1);
  display.setTextSize(2);
  display.setCursor(11, 9);
  display.setTextWrap(false);
  display.print(menuItems[option]); 
  display.drawRect(0, 0, 128, 32, 1);
  display.display();

  button1val = digitalRead(button1pin);
  button2val = digitalRead(button2pin);

  if(button1val == LOW && button1prevVal == HIGH){           //Zmiana opcji w menu
    option ++;
    if(option > 2){
      option = 0;
      }
  }

  if(button2val == LOW && button2prevVal == HIGH && option == 0){                       //Wybór opcji ćwiczenia
    exercise();
  }

  if(button2val == LOW && button2prevVal == HIGH && option == 1){
    results();
  }

  if(button2val == LOW && button2prevVal == HIGH && option == 2){
    options();
  }

  button1prevVal = button1val;  
  button2prevVal = button2val;
}
