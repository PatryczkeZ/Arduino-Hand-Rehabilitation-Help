#include <Adafruit_ADS1X15.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 //Screen width
#define SCREEN_HEIGHT 32 //Screen height
#define OLED_RESET     -1 //Screen reset
#define SCREEN_ADDRESS 0x3C //Screen address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Define screen

Adafruit_ADS1115 ads; //Define ads

unsigned int rectPossition; //Rectangle possition with estimated force
unsigned int rectSize = 12; //Rectangle size which depends on difficulty
unsigned long score = 0; //Define score variable
unsigned long currentTimeOnSpot = 0; //Variable which measure time when force on sensor is in the range of estimated force
unsigned int lastScore = 0; //Variable with last score
double meanScore = 0; //Variable with mean score
unsigned int sesionCounter = 0; //Variable which calculate sesions

//Time variables
unsigned long remTime = 0; 
unsigned long difTime = 0; 
unsigned long estTime = 10000;
unsigned long remClockTime = 0; 
unsigned long clockTime = 0; 

int button1pin = D3; //Arduino pins with buttons
int button2pin = D2; //
int button1val;      //Buttons states
int button2val;      //
int button1prevVal;  //Previous buttons states
int button2prevVal;  //   
int option = 0;      //Chosed option in menu
int a = 1;           //
int rectPossitionChange = 0; //Change rectangle possition 1 - true, 0 - false
const char *menuItems[] = {"Excercise", "Results", "Options"};  //Menu content

int chosedOption = 2; //Chosed option in menu (2 or 15 acording to pixels)
int difficulty = 1;   //Chosed difficulty (1, 2 or 3)


void options()  //Options menu
{
  delay(200); 
  while(a == 1){
    display.clearDisplay();
    display.drawRect(0, chosedOption, 128, 14, 1);
    display.setTextSize(1);
    display.setCursor(6, 5);
    display.print("Excercise time:");
    display.setCursor(6, 18);
    display.print("Difficulty:");
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

void results() //Results menu
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
    display.print("Last score:");
    display.setCursor(5, 17);
    display.print("Mean score:");
    display.setCursor(93, 5);
    display.print(lastScore);
    display.setCursor(93, 17);
    display.print(meanScore);
    display.display();
    
    //Leave menu
    if(button1val == LOW && button1prevVal == HIGH){  
      break;
    }
    //Clear results variables
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

void endSesion() //End of excercise
{
    display.clearDisplay();
    
    display.setTextColor(1);
    display.setTextSize(1);
    display.setCursor(10, 12);
    display.setTextWrap(false);               //Showing the information after excercise
    display.print("Your score: ");
    display.print(score/1000);
    display.display();
    
    sesionCounter ++;
    lastScore = score/1000;
    meanScore = ((meanScore * (sesionCounter - 1)) + score/1000)/sesionCounter;
    score = 0;
    
    delay(3000);
    
}

void exercise() //Excercise code
{
  score = 0;
  if(difficulty == 1){
    rectSize = 12;
    rectPossitionChange = 0;
  }
  if(difficulty == 2){                 //Rectangle size depends on difficulty variable
    rectSize = 10;
    rectPossitionChange = 0;
  }
  if(difficulty == 3){
    rectSize = 8;
    rectPossitionChange = 1;
  }
  rectPossition = random(81); //Randomize rectangle possition
  remClockTime = millis();    
  while(a == 1){
    int16_t adc0;
    float volts0;
    int16_t progress; //Variable with force on sensor

    difTime = millis() - remTime; //Calculate time difference
    adc0 = ads.readADC_SingleEnded(0); //Read information from adc
    volts0 = ads.computeVolts(adc0);
    if (volts0 < 0.44){
      progress = 0;
    }
    else if (volts0 >= 0.44 && volts0 <= 3.81){
      progress = 3.24*exp(1.37*volts0);
    }
    else{
      progress = 0.00005727*exp(4.25*volts0); //Calculate from volts to grams
    }

    clockTime = millis() - remClockTime; 

    if(rectPossitionChange == 1 && clockTime > estTime/2){
      rectPossition = random(81);                        //If difficulty == 3; change possition of rectangle in the middle of sesion
      rectPossitionChange = 0;
    }

    display.drawRect(rectPossition, 0, rectSize, 32, 1); //Draw rectangle with estimated force
    display.fillRect(0, 10, round(progress/10), 12, 1); //Draw a bar with force on sensor

    //Showing the score
    display.setTextColor(1);
    display.setTextSize(1);
    display.setCursor(103, 12);
    display.print(score/1000); 

    //Showing the force
    display.setCursor(95, 2);
    display.print(progress); 

    //Showing the time
    display.setCursor(102, 22);
    display.print((clockTime)/1000);

    display.display();

    
    if(progress/10 > rectPossition && progress/10 < rectPossition + rectSize){
      score += (millis() - currentTimeOnSpot); //Calculate score                                              
    }
    currentTimeOnSpot = millis(); 
    
    if (difTime >= 100){
        remTime = millis();         
    }

    if(clockTime > estTime){
      endSesion();                  //End of sesion 
      break;
    }

    display.clearDisplay();  //Clear screen
  }
}

void setup(void)
{

  Serial.begin(9600); 

  pinMode(button1pin, INPUT_PULLUP);  
  pinMode(button2pin, INPUT_PULLUP);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS."); 
    while (1);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));         
  }

  display.display();     
  display.clearDisplay();

}

void loop(void)  //Main program loop with the main menu
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

  if(button1val == LOW && button1prevVal == HIGH){           //Change menu option
    option ++;
    if(option > 2){
      option = 0;
      }
  }

  if(button2val == LOW && button2prevVal == HIGH && option == 0){            
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
