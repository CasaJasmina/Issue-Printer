/*
 This code connects to a Wifi network and
 prints out the comments and issues
 of a certain repository from github.com.
 It reads github's api as a string and parse it using
 the Json library.
 Circuit:
  Arduino zero
  WiFi 101 shield attached
*/

#include<WiFiSSLClient.h>
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>
#include "Adafruit_Thermal.h"
#include "HeaderNew.h"
#include "HeaderClosed.h"
#include "HeaderComment.h"
#include "footer.h"
#include "div3.h"
#include "div4.h"


Adafruit_Thermal printer(&Serial1);
#define TX_PIN 6 // Arduino transmit 
#define RX_PIN 5 // Arduino receive

const char* ssid = "My-Network";
const char* password = "My-Password";

const char* host = "api.github.com";  //this address is given by Github itself
const int httpsPort = 443;
WiFiSSLClient client;

String Old_id;
String IssueCommentEvent = "IssueCommentEvent";
String IssueEvent = "IssuesEvent";
String closed = "closed";
String line = "";
String line2 = "";
int counter = 0;
int a = 0;



void setup() {     ///////////////////////////////////////////CONNECTING TO INTERNTET/////////////////////////////////////
  pinMode(7, OUTPUT); digitalWrite(7, LOW);
  Serial1.begin(9600);
  Serial.begin(9600);
  //  while (!Serial);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //  Use WiFiClientSecure class to create TLS connection
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }}

                                            
void httpRequest() {                        ////////////////////////////////////// HTTP CONNECTION //////////////////////////////////////////

  if (!client.connect(host, httpsPort)) {    //Connect to github
    //    Serial.println("connection failed");
    return;
  }

  String url = "/orgs/casajasmina/events?page=1&per_page=1";  //   /orgs/myRepository/events
  Serial.print("requesting URL: ");                           //  " ?page=1&per_page=1 " add this to get events
  Serial.println(url);                                        //    one by one avoiding the filling of SRAM memory

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +       //send an HTTP request
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n\r\n");
  // "Connection: close\r\n\r\n");
  Serial.println("request sent");


}


void loop() {                   ///////////////////////////////////////// LOOP ///////////////////////////////////////
  int q = 0;
  DynamicJsonBuffer  jsonBuffer; //Define the space in the memory for the Json parsing as dynamic since you don't know its dimension
  httpRequest();
  String line = "";             // line is the string where the github response will be saved

  while (client.connected()) {
    line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

    
  line = client.readStringUntil('\n');
  JsonArray& root = jsonBuffer.parseArray(line);
  
  if (!root.success()) {
    Serial.println("parseArray() failed");
  } 


  String id = root[0]["id"];   //Compare id to not print old events
  String type = root[0]["type"];
  if (id.equals(Old_id)) {
    Serial.println("There are no new events");
  }
  else {
    printer.begin(200);

    if (type.equals(IssueCommentEvent)) {
      IssueComment(root);
      Serial.println("closing connection");
      Old_id = id;
    }

    else if (type.equals(IssueEvent)) {
      NewIssue(root);
      Serial.println("closing connection");
      Old_id = id;
    }

    printer.sleep();      // Tell printer to sleep
    delay(3000L);         // Sleep for 3 seconds
    printer.wake();       // MUST wake() before printing again, even if reset
    printer.setDefault(); // Restore printer to defaults
  }

  line.remove(0);
  client.flush();
  client.stop();

  delay(60 * 1000); //github allows you to make only 60 request per hour
}


void IssueComment(JsonArray & root1) {  //////////////////////////////////////// COMMENT-EVENT ////////////////////////////////////////

  String repo = root1[0]["repo"]["name"];
  String title = root1[0]["payload"]["issue"]["title"];
  String login = root1[0]["actor"]["login"];
  String body = root1[0]["payload"]["comment"]["body"];
  String data = root1[0]["payload"]["issue"]["created_at"];
  printer.setLineHeight(30);
  PrinterWrap(title);   

  printer.printBitmap(HeaderComment_width, HeaderComment_height, HeaderComment_data);
  PrinterWrapM(body); 
  printer.printBitmap(div4_width, div4_height, div4_data);

  printer.setSize('S');
  printer.justify('C');
  printer.boldOn();
  printer.println(repo);
  printer.boldOff();
  printer.println(login);
  printer.println(data.substring(0, 10));

  String label = root1[0]["payload"]["issue"]["labels"][0]["name"];

  if (label.equals("")) {
    printer.printBitmap(footer_width, footer_height, footer_data);
  }
  else {
    printer.printBitmap(div3_width, div3_height, div3_data);
    while (true) {
      String label2 = root1[0]["payload"]["issue"]["labels"][counter]["name"];
      if (label2.equals("")) {
        break;
      }
      printer.inverseOn();
      printer.println(" " + label2 + " ");
      printer.inverseOff();
      counter++;
    }
  }
  printer.println("");
}



void NewIssue(JsonArray & root1) {        ////////////////////////////////////////// NEW ISSUE //////////////////////////////////////

  String action = root1[0]["payload"]["action"];



  printer.setLineHeight(30);

  String repo = root1[0]["repo"]["name"];
  String title = root1[0]["payload"]["issue"]["title"];
  String login = root1[0]["actor"]["login"];
  String body = root1[0]["payload"]["issue"]["body"];
  String data = root1[0]["payload"]["issue"]["created_at"];
  String data2 = root1[0]["payload"]["issue"]["closed_at"];


  PrinterWrap(title);
  if (action.equals("closed")) {
    printer.printBitmap(HeaderClosed_width, HeaderClosed_height, HeaderClosed_data);
  }
  else {
    printer.printBitmap(HeaderNew_width, HeaderNew_height, HeaderNew_data);
    printer.setLineHeight(30);
    if (!body.equals("")) {
      PrinterWrapM(body);
      printer.printBitmap(div4_width, div4_height, div4_data);
    }
  }
  printer.setSize('S');
  printer.justify('C');
  printer.boldOn();
  printer.println(repo);
  printer.boldOff();
  printer.println(login);
  if (action.equals("closed")) {
    printer.println(data2.substring(0, 10));}
  else{
  printer.println(data.substring(0, 10));}

  String label = root1[0]["payload"]["issue"]["labels"][0]["name"];
  if (label.equals(" ")) {
    printer.printBitmap(footer_width, footer_height, footer_data);
  }
  else {
    printer.printBitmap(div3_width, div3_height, div3_data);
    printer.inverseOn();
    printer.println(" " + label + " ");
    printer.inverseOff();
  }
  printer.println("");
  printer.println("");
  printer.println("");
  printer.println("");
}






void PrinterWrap(String StringTwo) {       //////////This method is used to get the right words layout using L size//////////

  int spacezero = 0;
  int lastspace = StringTwo.lastIndexOf(' ');
  int nextspace = 0;
  int firstspace = StringTwo.indexOf(' ');

  for (int b = 0; b <= StringTwo.length(); b++) {
    nextspace = StringTwo.indexOf(' ', firstspace + b );
    if (nextspace - spacezero > 14) {
      int previous = StringTwo.indexOf(' ', firstspace + (b - 1) );
      if (spacezero == 0) {
        printer.setSize('L');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero, previous));
      }
      if (spacezero != 0) {
        printer.setSize('L');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero + 1, previous));
      }

      spacezero = previous;
    }
    if (nextspace - spacezero == 15) {
      if (spacezero == 0) {
        printer.setSize('L');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero, nextspace));
      }
      if (spacezero != 0) {
        printer.setSize('L');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero + 1, nextspace));
      }
      spacezero = nextspace;
    }
    if (b == StringTwo.length()) {
      if (StringTwo.charAt(spacezero) == ' ') {
        printer.setSize('L');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero + 1));
      }
      else {
        printer.setSize('L');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero));
      }
    }
  }
}
void PrinterWrapM(String StringTwo) {       //////////This method is used to get the right words layout using M size//////////

  int spacezero = 0;
  int lastspace = StringTwo.lastIndexOf(' ');
  int nextspace = 0;
  int firstspace = StringTwo.indexOf(' ');

  for (int b = 0; b <= StringTwo.length(); b++) {
    nextspace = StringTwo.indexOf(' ', firstspace + b );
    if (nextspace - spacezero > 30) {
      int previous = StringTwo.indexOf(' ', firstspace + (b - 1) );
      if (spacezero == 0) {
        printer.setSize('M');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero, previous));
      }
      if (spacezero != 0) {
        printer.setSize('M');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero + 1, previous));
      }

      spacezero = previous;
    }
    if (nextspace - spacezero == 30) {
      if (spacezero == 0) {
        printer.setSize('M');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero, nextspace));
      }
      if (spacezero != 0) {
        printer.setSize('M');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero + 1, nextspace));
      }
      spacezero = nextspace;
    }
    if (b == StringTwo.length()) {
      if (StringTwo.charAt(spacezero) == ' ') {
        printer.setSize('M');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero + 1));
      }
      else {
        printer.setSize('M');
        printer.justify('L');
        printer.println(StringTwo.substring(spacezero));
      }
    }
  }
}





