# Issue-Printer
### Bringing GitHub issues in the physical world
The Issue-Printer is a little thermal printer, connected to Arduino, that automatically prints out comments and issues from a certain repository of GitHub.

<img src="https://github.com/CasaJasmina/Issue-Printer/blob/master/Img/printhorizontal.jpg" width="800" /> 

### Connecting the printer

In order to connect the printer to the Arduino you only need three wires: the red, the black and the blue one.</br>
The red one goes in the vin pin and provides the current</br>The black one is for the ground</br>The blue one, which is where datas pass through, goes in the tx pin.



### Loading sketches

Before loading any sketch you need to install the [adafruit library](https://github.com/adafruit/Adafruit-Thermal-Printer-Library) and the [json library](https://github.com/bblanchon/ArduinoJson).
In this repository you will find an [Arduino sketch](https://github.com/CasaJasmina/Issue_ThermalPrinter/tree/master/Arduino_Github_Printer) ready to be loaded.
All the .h files are bitmaps of images made with the processing sketch that you can find in the Adafruit's library examples.




### BOM

| part		        | Quantity | Link |
| -------------     |:--------:| :--------:| 
| Arduino zero        | 1 | https://store.arduino.cc/product/GBX00003 | 
| Thermal Printer      | 1 | http://www.aliexpress.com/item/Embedded-thermal-printer-all-in-POS-driving-recorder-medical-equipment/1894510380.html | 
| WiFi 101 shield   | 1 | http://store-usa.arduino.cc/products/asx00001	| 
| Power Adapter 5. 0Vdc/5000mA   | 1 |  | 





