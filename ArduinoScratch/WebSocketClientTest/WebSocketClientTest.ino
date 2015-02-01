#include "Arduino.h"
#include <WebSocketClient.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <Adafruit_NeoPixel.h>


// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "Sebs"           // cannot be longer than 32 characters!
#define WLAN_PASS       "Internet"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define CRLF "\r\n"
// What page to grab!
#define SERVER      "10.0.1.32"
#define PATH         "/"
#define PORT         8001

#define RED     0x110000
#define ORANGE  0x118800
#define GREEN   0x001100
#define BLACK   0x000000

#define RECONNECT_SWITCH A8

/**************************************************************************/
/*!
 @brief  Sets up the HW and the CC3000 module (called automatically
 on startup)
 */
/**************************************************************************/

uint32_t ip;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(2, A14, NEO_GRB + NEO_KHZ800);

Adafruit_CC3000_Client client;

unsigned long lastSend;

String msg; 

const char BEGINCHAR = (char)0;
const char ENDCHAR = (char)255;


byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//char server[] = "node.seb.ly";
WebSocketClient wsclient;

void setup() {

  pinMode(RECONNECT_SWITCH, INPUT); 

  Serial.begin(115200);


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

  if(millis() - lastSend > 1000) { 
    //  Serial.println("Sending Hey!"); 
    //wsclient.send("Hey!");
    wsclient.send("{\"type\" : \"heartbeat\"}");
    lastSend = millis();
  }

  if(client.connected()) socketGreenLight(); 
  else socketRedLight();

  if(digitalRead(RECONNECT_SWITCH)) { 
    wsclient.disconnect();  
    wsclient.connect(&client, SERVER, PATH, PORT);
    wsclient.send("{\"type\" : \"register\"}");
  }

}

void dataArrived(WebSocketClient wsclient, String data) {
  Serial.println("Data Arrived: " + data);
  
  
  
}

