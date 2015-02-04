
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "ST4iWifiManager.h"
#include <WebSocketClient.h>
#include <JsonParser.h>
using namespace ArduinoJson::Parser;

ST4iWifiManager wifi; 

#define SERVER      "node.seb.ly"
#define PATH         "/"
#define PORT         8102

#define WIFI "Sebs"
#define PASSWORD "Internet"

// Change this to YOUR name!
String group = String("Seb");

JsonParser<100> parser;

const int ledPin = 44; 
const int buttonPin = 21;
const int wifiLed = 2; 
const int buzzerPin = 34; 
const int heartbeatFrequency = 20000; 

boolean buttonPushed = false; 

uint32_t ip;
unsigned long lastSend;

WebSocketClient wsclient;

void setup() {
  Serial.begin(115200);

  wifi.init(wifiLed, 30); 
  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 

  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT); 

  digitalWrite(ledPin, HIGH); 
  delay(1000); 
  digitalWrite(ledPin, LOW);


 
  connectWebsocket(); 

 

}

void loop() {

  // check wifi !
  wifi.monitor(); 
  // check websocket
  wsclient.monitor();
  
  if(!wsclient.connected()) {
    Serial.println("Websocket disconnected, reconnecting");
     // try reconnecting!  
     connectWebsocket(); 
  }

  // heartbeat code to keep the connection alive 
  if(millis() - lastSend > heartbeatFrequency) { 
    Serial.print("*"); 
    wsclient.send(String("{\"type\" : \"heartbeat\", \"group\" : \"")+group+String("\"}"));
    lastSend = millis();
   
  }


  if(!digitalRead(buttonPin) != buttonPushed) { 
    buttonPushed = !buttonPushed; 
    String msg; 

    if(buttonPushed) 
      msg = String("{\"type\" : \"light\", \"data\" : 1, \"group\" : \"")+group+String("\"}");
    else 
      msg = String("{\"type\" : \"light\", \"data\" : 0, \"group\" : \"")+group+String("\"}");
    Serial.print("sending : "); 
    Serial.println(msg); 

    wsclient.send(msg);
    Serial.println("sent");
  }


}

void dataArrived(WebSocketClient wsclient, String data) {

  char jsonchar[data.length()+1];
  data.toCharArray(jsonchar, data.length()+1);

  // should poss parse json? 
  JsonObject json = parser.parse(jsonchar);
  Serial.println("Data Arrived: " + data);

  const char* msggroup = json["group"]; 

  if(strcmp(msggroup, group.c_str())!=0) return; 

  const char* type = json["type"]; 

  Serial.println(type); 
  if(strcmp(type,"light")==0) { 

    Serial.println("found light"); 
    int state = json["data"]; 

    if(state==1) {
      digitalWrite(ledPin, HIGH); 
      tone(buzzerPin,294);

    } 
    else {
      digitalWrite(ledPin, LOW); 
      noTone(buzzerPin);
    }
  } 
  else if(strcmp(type,"message")==0) { 

    Serial.println("found message"); 
    char* msg = json["data"]; 
    Serial.println(msg);
    
  }
}


void connectWebsocket() { 
  
  wsclient = WebSocketClient(); 
  Serial.println("connecting to socket server");

  while(!wsclient.connect(&wifi.client, SERVER, PATH, PORT)) { 
    Serial.println("couldn't connect :( Trying again..."); 
    delay(1000);  
  }
  Serial.println("Connected to Websocket server!"); 

  Serial.println("Socket connected!"); 
  wsclient.setDataArrivedDelegate(dataArrived);
  String sendstring = String(String("{\"type\" : \"register\", \"group\" : \"")+group+String("\"}"));
  wsclient.send(sendstring);
  lastSend = millis(); 

}


