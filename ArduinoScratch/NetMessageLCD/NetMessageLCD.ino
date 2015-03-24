
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "WifiManager.h"
#include <WebSocketClient.h>
#include <JsonParser.h>
using namespace ArduinoJson::Parser;

#include <LiquidCrystal.h>
LiquidCrystal lcd(19,18,17,16,15,14);

WifiManager wifi; 

#define SERVER      "node.seb.ly"
#define PATH         "/"
#define PORT         8103


#define WIFI "MyWifiNetwork"
#define PASSWORD "MyPassword"

// Change this to YOUR name!
String id = String("Seb");


JsonParser<100> parser;

const int ledPin = 44; 
const int buttonPin = 21;
const int wifiLed = 2; 
const int heartbeatFrequency = 20000; 

boolean buttonPushed = false; 

uint32_t ip;
unsigned long lastSend;

WebSocketClient wsclient;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Connecting...");
  Serial.begin(115200);

  // wifi.init(int okLedPin, int brightness (0 to 255, but only works on PWM pins))
  wifi.init(wifiLed, 30); 
  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 

  lcd.setCursor(0,1);
  lcd.print("Wifi OK");



  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT); 

  digitalWrite(ledPin, HIGH); 
  delay(1000); 
  digitalWrite(ledPin, LOW);


  Serial.println("connecting to socket server");


  while(!wsclient.connect(&wifi.client, SERVER, PATH, PORT)) { 
    Serial.println("couldn't connect :( Trying again..."); 
    delay(1000);  

  }

  lcd.setCursor(0,1);
  lcd.print("Socket Connected");


  Serial.println("Socket connected!"); 
  wsclient.setDataArrivedDelegate(dataArrived);
  String sendstring = String(String("{\"type\" : \"register\", \"id\" : \"")+id+String("\"}"));
  wsclient.send(sendstring);
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
    String msg; 

    if(buttonPushed) 
      msg = String("{\"type\" : \"light\", \"data\" : 1, \"id\" : \"")+id+String("\"}");
    else 
      msg = String("{\"type\" : \"light\", \"data\" : 0, \"id\" : \"")+id+String("\"}");

    wsclient.send(msg);
  }


}

void dataArrived(WebSocketClient wsclient, String data) {

  char jsonchar[data.length()+1];
  data.toCharArray(jsonchar, data.length()+1);

  // should poss parse json? 
  JsonObject json = parser.parse(jsonchar);
  Serial.println("Data Arrived: " + data);

  const char* msgid = json["id"]; 

  if(strcmp(msgid, id.c_str())!=0) return; 

  const char* type = json["type"]; 

  Serial.println(type); 
  if(strcmp(type,"light")==0) { 

    Serial.println("found light"); 
    int state = json["data"]; 

    if(state==1) {
      digitalWrite(ledPin, HIGH); 
      tone(9,294);

    } 
    else {
      digitalWrite(ledPin, LOW); 
      noTone(9);
    }
  } 
  else if(strcmp(type,"message")==0) { 

    Serial.println("found message"); 
    char* msg = json["data"]; 
    Serial.println(msg);
    printString(msg); 
  }
}

void printString(char * msg) { 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);


}








