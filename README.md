
## About The Project

For my Engineering Degree, I created a hand rehabilitation device. It measures the force between fingers and displays a simple grip strength game on the Arduino display. The game involves the user achieving and holding the strength within a range drawn at the beginning of the session.
The user can change the difficulty, see the results of the last session, and view the average score from all sessions since turning on the device.


The device is an attempt to create the possibility of hand rehabilitation at home for patients with various conditions that limit its motor skills.

## Scheme And Used Components
![image](https://github.com/PatryczkeZ/Arduino-Hand-Rehabilitation-Help/assets/124098053/365e1696-ff68-4a6e-ac57-6707cf977b94)


* [FSR-402](https://botland.store/czujniki-nacisku/753-czujnik-sily-nacisku-fsr-402-2kg-okragly-18mm-pololu-1696-5904422366896.html)
* [Arduino compatibile module](https://botland.store/withdrawn-products/5863-d1-r2-wifi-esp8266-compatible-with-wemos-and-arduino.html)
* [ADS1115 16bit](https://botland.store/raspberry-pi-gpio-extensions/8444-ads1115-16-bit-adc-4-channel-with-programmable-gain-amplifier-stemma-qtqwiic-adafruit-1085-5904422311728.html)
* [Oled Screen 0.91"](https://www.winstar.com.tw/products/oled-module/graphic-oled-display/oled-i2c.html)
* [3kOhm Resistor](https://www.ebay.com/itm/324682607222?itmmeta=01HSBV3V68Y8GHT34P9FZX1WFS&hash=item4b98976a76:g:lLMAAOSwcXhgzN3l&itmprp=enc%3AAQAJAAAA4LdaryVerAxSsBITtwZ%2FWTVisgrB0dbxS0VrxLZXKbjo9IRVABQbOyYmQpaHuRCFeN7nw3dGYxz5V6CyV1lgLLgVQq3qT0ia2p00SiYcCgXIP1EumsBYQBGE%2F6uDw818LJHjikSBdHOkcW2C%2Bcze%2Bxv2btBwqZDt15dixemnBl6DOnrkoTqsxewTFuXZoov2yHMyu2DnCnqhIszno%2FVERpaQMo7BnrVcL8IUsfOVsS3BqQVR%2FPcjlUumFaqpNBV3JhrLDW54dwTXub8Cr3uWxlaFMlTsHURSVx6uWz90dwg%2F%7Ctkp%3ABFBMtLOP-8pj)
* [Buttons](https://www.ebay.com/itm/194197615826?itmmeta=01HSBV51EWF804SFHQ6EFQ9TGD&hash=item2d371474d2:g:oHAAAOSwHF9gwxuP&itmprp=enc%3AAQAJAAAA4Id8jaz%2BepTifNRm6%2FZLQGsvnF8DopE8e3s4TxTZjWbRoWeU8LuW1dNLH%2FHEqv%2B3D5nXuYxjc%2BArAZOpOy%2B2To2AA59Edh1Xm4C17anAEUGUlgTZJNjh%2F98OdVz%2FN8i51hAwM2mPZ9sh6WzfSizjNjFRTof5laYn5Dd%2FpJPAjsir2aaFkLOsVc0dqydrf3ngK1lMVAnqqcuSv08ihWJjAGR0wE2jr7yVLRkoZmHs2PukGMx0IriNpqyTSJdY9Nyv5%2BksgAtEIRUTiffzzNF18ZyqhmYGDBSL24Og6D%2Bvgl75%7Ctkp%3ABk9SR9qXlPvKYw)

## Device
#### Constructed Device
![image](https://github.com/PatryczkeZ/Arduino-Hand-Rehabilitation-Help/assets/124098053/02725c8a-f61b-4158-b94d-83f8082cf188)

#### Excercise
![image](https://github.com/PatryczkeZ/Arduino-Hand-Rehabilitation-Help/assets/124098053/475407d3-1f84-4f36-8b45-9d4afa7aa969)


The user need to hold the force in the range of rectangle. 


* Number on top shows current force measured by sensor.
* Second number is current score.
* Last number is session time.

Score = time when the horizontal bar is within the rectangle.

<p align="center">
Thanks for reading!
</p>





