
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "WifiManager.h"
#include <WebSocketClient.h>

WifiManager wifi; 

#define SERVER      "node.seb.ly"
#define PATH         "/"
#define PORT         8101


#define WIFI "indi-wifi"
#define PASSWORD "id1w679mv"


const int redPin = 44; 
const int greenPin = 45; 
const int bluePin = 46; // 44 Red, 45 Green, 46 Blue 

const int buttonPin = 21;
const int wifiLed = 2; 
const int heartbeatFrequency = 20000; 

boolean buttonPushed = false; 

uint32_t ip;
unsigned long lastSend;

WebSocketClient wsclient;

void setup() {

  Serial.begin(115200);
  
  // wifi.init(int okLedPin, int brightness (0 to 255, but only works on PWM pins))
  wifi.init(wifiLed, 30); 
  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 
   
  pinMode(ledPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 

  pinMode(buttonPin, INPUT); 

  Serial.println("connecting to socket server");

  // **** break this out into a function and call it on a disconnect
  while(!wsclient.connect(&wifi.client, SERVER, PATH, PORT)) { 
    Serial.println("couldn't connect :( Trying again..."); 
    delay(1000);  

  }
  Serial.println("Socket connected!"); 
  // socketGreenLight();
  wsclient.setDataArrivedDelegate(dataArrived);
  wsclient.send("{\"type\" : \"register\"}");
  lastSend = millis(); 

}

void loop() {

  // check wifi !
  wifi.monitor(); 
  // check websocket
  wsclient.monitor();

  // heartbeat code to keep the connection alive 
  if(millis() - lastSend > heartbeatFrequency) { 
    Serial.print("*"); 
    wsclient.send("{\"type\" : \"heartbeat\"}");
    lastSend = millis();
  }


  if(!digitalRead(buttonPin) != buttonPushed) { 
    buttonPushed = !buttonPushed; 
    if(buttonPushed) wsclient.send("{\"type\" : \"light\", \"data\" : \"on\"}");
    else wsclient.send("{\"type\" : \"light\", \"data\" : \"off\"}");
    
  }

  //  if(client.connected()) socketGreenLight(); 
  //  else socketRedLight();

  //  if(digitalRead(RECONNECT_SWITCH)) { 
  //    wsclient.disconnect();  
  //    socketRedLight();
  //    delay(100); 
  //    while(digitalRead(RECONNECT_SWITCH));
  //
  //    wsclient.connect(&client, SERVER, PATH, PORT);
  //    wsclient.send("{\"type\" : \"register\"}");
  //  }

}

void dataArrived(WebSocketClient wsclient, String data) {
  
  Serial.println("Data Arrived: " + data);
  
  if(data.indexOf("on")>-1) {
    digitalWrite(ledPin, HIGH); 
    tone(9,294);

  } 
  else if (data.indexOf("off")>-1) {
    digitalWrite(ledPin, LOW); 
    noTone(9);
  }

}




