
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "ST4iWifiManager.h"
#include <WebSocketClient.h>

ST4iWifiManager wifi; 

#define SERVER      "node.seb.ly"
#define PATH         "/"
#define PORT         8101

#define WIFI "LeanDUS"
#define PASSWORD "LeanDUS2015"

const int redPin = 44; 
const int greenPin = 45;
const int bluePin = 46;

const int buttonPin = 21;
const int wifiLed = 2; 
const int heartbeatFrequency = 20000; 

boolean buttonPushed = false; 

uint32_t ip;
unsigned long lastSend;

WebSocketClient wsclient;

void setup() {
  Serial.begin(115200);

  wifi.init(wifiLed, 30); 
  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 

  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  pinMode(buttonPin, INPUT); 

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
  if((unsigned long) (millis() - lastSend) > heartbeatFrequency) { 
    Serial.print("*"); 
    wsclient.send("{\"type\" : \"heartbeat\"}");
    lastSend = millis();

  }


  if(!digitalRead(buttonPin) != buttonPushed) { 
    buttonPushed = !buttonPushed; 
    String msg; 

    if(buttonPushed) wsclient.send("{\"type\":\"light\", \"data\":\"on\"}");
    else wsclient.send("{\"type\":\"light\",\"data\":\"off\"}");
  }


}

void dataArrived(WebSocketClient wsclient, String data) {

  Serial.println("Data Arrived: " + data);

  if(data.indexOf("\"on\"")>-1) {
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0); 
  } 
  else if (data.indexOf("off")>-1) {
    digitalWrite(redPin, LOW); 
    digitalWrite(greenPin, LOW); 
    digitalWrite(bluePin, LOW); 
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
  lastSend = millis(); 

}



