/*
 This code connects to an unencrypted Wifi network and
 print out the events of a certain repository from github.com.
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

const char* ssid = "CasaJasmina";
const char* password = "c0nn3ct3d";

const char* host = "api.github.com";  //this address is given by Github itself
const int httpsPort = 443;
WiFiSSLClient client;
String Old_id;
String IssueCommentEvent = "IssueCommentEvent";
String IssueEvent = "IssuesEvent";
String PushEvent = "PushEvent";
String closed = "closed";



void setup() {     ////////////////////////////////////////////////////////////////////////////////

  Serial.begin(9600);
  while (!Serial);
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

  // Use WiFiClientSecure class to create TLS connection
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {   ////////////////////////////////////////////////////////////////////////////////

  if (!client.connect(host, httpsPort)) {    //Connect to github
    Serial.println("connection failed");
    return;
  }

  String url = "/orgs/casajasmina/events?page=1&per_page=1";  //   /orgs/myRepository/events
  Serial.print("requesting URL: ");                           //  " ?page=1&per_page=1 " add this to get events
  Serial.println(url);                                        //    one by one and not to fill SRAM memory

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +       //send an HTTP request
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n\r\n");
  // "Connection: close\r\n\r\n");
  Serial.println("request sent");


}


void loop() {    ////////////////////////////////////////////////////////////////////////////////

  DynamicJsonBuffer  jsonBuffer; //Define the space in the memory for the Json parsing as dynamic since you don't know its dimension

  httpRequest();
  String line = ""; // line is the string where the github response will be saved

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
  String type = root[0]["type"]; // Check the type of the event

  if (id.equals(Old_id)) {
    Serial.println("There are no new events");
  }
  else {

    if (type.equals(IssueCommentEvent)) {
      Serial.println("==========");
      IssueComment(root);
      Serial.println("==========");
      Serial.println("closing connection");
      Old_id = id;
    }

    if (type.equals(IssueEvent)) {
      Serial.println("==========");
      NewIssue(root);
      Serial.println("==========");
      Serial.println("closing connection");
      Old_id = id;
    }
  }

  client.flush();
  client.stop();

  delay(15 * 1000); //github allows you to make only 60 request per hour
}



void IssueComment(JsonArray& root1) {   ////////////////////////////////////////////////////////////////////////////////

  Serial.println("New Comment!: ");

  Serial.print("Repo : ");
  String repo = root1[0]["repo"]["name"];
  Serial.println(repo);

  Serial.print("Title: ");
  String title = root1[0]["payload"]["issue"]["title"];
  Serial.println(title);

  Serial.print("Posted by : ");
  String login = root1[0]["actor"]["login"];
  Serial.println(login);

  Serial.print("Content: ");
  String body = root1[0]["payload"]["comment"]["body"];
  Serial.println(body);

}

void NewIssue(JsonArray& root1) {         ////////////////////////////////////////////////////////////////////////////////

  String action = root1[0]["payload"]["action"];

  if (action.equals("closed")) {

    Serial.println("Issue closed!");

    Serial.print("Repo: ");
    String repo = root1[0]["repo"]["name"];
    Serial.println(repo);

    Serial.print("Title: ");
    String title = root1[0]["payload"]["issue"]["title"];
    Serial.println(title);

    Serial.print("Closed by : ");
    String login = root1[0]["actor"]["login"];
    Serial.println(login);

    Serial.print("Content: ");
    String body = root1[0]["payload"]["issue"]["body"];
    Serial.println(body);

    Serial.print("Closed at: ");
    String data = root1[0]["payload"]["issue"]["created_at"];
    Serial.println(data);

  }
  else {
    Serial.print("New Issue in: ");

    String repo = root1[0]["repo"]["name"];
    Serial.println(repo);

    Serial.print("Title: ");
    String title = root1[0]["payload"]["issue"]["title"];
    Serial.println(title);

    Serial.print("Posted by : ");
    String login = root1[0]["actor"]["login"];
    Serial.println(login);

    Serial.print("Content: ");
    String body = root1[0]["payload"]["issue"]["body"];
    Serial.println(body);

    Serial.print("Number: ");
    String number = root1[0]["payload"]["issue"]["number"];
    Serial.println(body);

    Serial.print("Created at: ");
    String data = root1[0]["payload"]["issue"]["created_at"];
    Serial.println(data);
  }
}



