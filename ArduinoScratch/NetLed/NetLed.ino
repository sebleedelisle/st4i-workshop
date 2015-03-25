#include "Arduino.h"
#include <WebSocketClient.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>


// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "indi-wifi"           // cannot be longer than 32 characters!
#define WLAN_PASS       "id1w679mv"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define CRLF "\r\n"


#define SERVER      "10.0.1.32"
#define PATH         "/"
#define PORT         8000

#define RED     0x080000
#define ORANGE  0x080400
#define GREEN   0x000800
#define BLACK   0x000000

#define RECONNECT_SWITCH A8



uint32_t ip;
//Adafruit_NeoPixel leds = Adafruit_NeoPixel(2, A14, NEO_GRB + NEO_KHZ800);

Adafruit_CC3000_Client client;

unsigned long lastSend;

const int ledPin = A9;

WebSocketClient wsclient;

void setup() {

  pinMode(RECONNECT_SWITCH, INPUT); 
  pinMode(ledPin, OUTPUT); 

  digitalWrite(ledPin, HIGH); 
  delay(1000); 
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  tone(9,294, 500);

  initLEDs();
  initCC3000(); 
  connectToWifi();   

  Serial.println("connecting to socket server");


  while(!wsclient.connect(&client, SERVER, PATH, PORT)) { 
    Serial.println("couldn't connect :( Trying again..."); 
    delay(1000);  

  }
  Serial.println("Socket connected!"); 
  socketGreenLight();
  wsclient.setDataArrivedDelegate(dataArrived);
  wsclient.send("{\"type\" : \"register\"}");
  lastSend = millis(); 

}

void loop() {
  wsclient.monitor();

  if(millis() - lastSend > 5000) { 
    Serial.print("*"); 
    //wsclient.send("Hey!");
    wsclient.send("{\"type\" : \"heartbeat\"}");
    lastSend = millis();
  }

  if(client.connected()) socketGreenLight(); 
  else socketRedLight();

  if(digitalRead(RECONNECT_SWITCH)) { 
    wsclient.disconnect();  
    socketRedLight();
    delay(100); 
    while(digitalRead(RECONNECT_SWITCH));

    wsclient.connect(&client, SERVER, PATH, PORT);
    wsclient.send("{\"type\" : \"register\"}");
  }

}

void dataArrived(WebSocketClient wsclient, String data) {
  Serial.println("Data Arrived: " + data);
  if(data.indexOf("on")>-1) {
    digitalWrite(ledPin, HIGH); 
    tone(9,294);

  } else if (data.indexOf("off")>-1) {
    digitalWrite(ledPin, LOW); 
    noTone(9);
  }

}



