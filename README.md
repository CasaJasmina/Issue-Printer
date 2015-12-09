# Issue_Thermal_Printer
### Bringing GitHub issues in the physical world
The Issue_Thermal_Printer is a little printer, connected to Arduino, that automatically prints out comments, commits and issues from GitHub.

<img src="https://github.com/CasaJasmina/Issue_ThermalPrinter/blob/master/Img/Printer.jpg " width="400" /> 

## Connecting the printer

In order to connect the printer to the Arduino you only need three wires: the red, the balck and the blue one.
The red one goes in the vin pin and is needed for the power supply, the balck one is for the ground and the blue one, which is where datas pass through, goes in the tx pin.



## Loading sketches

Before loading any sketch you need to install the [adafruit library](https://github.com/adafruit/Adafruit-Thermal-Printer-Library) and the [json library](https://github.com/bblanchon/ArduinoJson).

In this repository you will find an Arduino sketch ready to be loaded([Arduino code](https://github.com/CasaJasmina/Issue_ThermalPrinter/tree/master/Arduino_Github_Printer)).
All the .h files are bitmap of images made with the processing sketch that you can find in the Adafruit's library examples.



## Creating the enclosure



### Notes about the sketch

The Arduino code connects to an unencrypted Wifi network and
print out the events of a certain repository from github.com.
It reads github's api as a string and parse it using
the Json library.
Depending on the size of the github response you might not be able to parse it or even read it.
In this case you can read only a part of the string and pare only that part. This method assume you already know 
what kinf of response you are going to recieve.





## BOM

| part		        | Quantity | Link |
| -------------     |:--------:| :--------:| 
| Arduino zero        | 1 | https://store.arduino.cc/product/GBX00003 | 
| Thermal Printer      | 1 | http://www.aliexpress.com/item/Embedded-thermal-printer-all-in-POS-driving-recorder-medical-equipment/1894510380.html | 
| WiFi 101 shield   | 1 | http://store-usa.arduino.cc/products/asx00001	| 
| power supply    | 1 | CF18JT100KCT-ND | 



##Credits


