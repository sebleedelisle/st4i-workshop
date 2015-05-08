#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "ST4iWifiManager.h"
#include <WebSocketClient.h>
#include <JsonParser.h>
using namespace ArduinoJson::Parser;

#include <LiquidCrystal.h>
LiquidCrystal lcd(19,18,17,16,15,14);

ST4iWifiManager wifi; 

int numPixels = 12; 
int rgb[] = {0,0,0}; 
Adafruit_NeoPixel strip(numPixels, 22,  NEO_GRB + NEO_KHZ800); 



#define SERVER      "node.seb.ly"
#define PATH         "/"
#define PORT         8102

#define WIFI "LeanDUS"
#define PASSWORD "LeanDUS2015"

// Change this to YOUR name!
String id = String("Seb");

JsonParser<100> parser;

const int ledPin = 44; 
const int buttonPin = 21;
const int wifiLed = 2; 
const int buzzerPin = 34; 
const int heartbeatFrequency = 20000; 

boolean buttonPushed = false; 

uint32_t ip;
unsigned long lastSend;

int hue = 0; 
int targetHue = 0; 
int saturation = 100; 
  

WebSocketClient wsclient;

void setup() {
  Serial.begin(115200);
  
  strip.begin(); 
  strip.show(); 
  
  showColour(0xff00ff); 
  
  // 16 x 2 characters
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Connecting...");
 
  wifi.init(wifiLed, 30); 
  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 

  lcd.setCursor(0,1);
  lcd.print("Wifi OK");

  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT); 

  digitalWrite(ledPin, HIGH); 
  delay(1000); 
  digitalWrite(ledPin, LOW);


 
  connectWebsocket(); 
  lcd.setCursor(0,1);
  lcd.print("Socket Connected");

 

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
    wsclient.send(String("{\"type\" : \"heartbeat\", \"id\" : \"")+id+String("\"}"));
    lastSend = millis();
   
  }

  
  if(hue<targetHue) hue++; 
  else if(hue>targetHue) hue--; 
  
  for(int i = 0; i<numPixels; i++) { 
    int brightness = map(sin((double)(millis()+(i*100))*0.01f ) * 100, -100, 100, 0,100);
    setPixelHSB(i, hue, saturation, brightness); 
  } 
  strip.show(); 
  


}

void showColour(uint32_t c) { 
  
   for(int i = 0; i<numPixels; i++) { 
    
      strip.setPixelColor(i, c); 
    
   } 
   strip.show(); 
  
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
    showStringOnLCD(msg); 
    
    if(strcmp(msg, "red") == 0) { 
      
       //showColour(0xff0000);  
      
       targetHue = 0; 
       
    } else if(strcmp(msg, "blue") == 0) { 
      targetHue = 210; 
       //showColour(0x00ff00);  
      
    } else if(strcmp(msg, "cyan") == 0) { 
      targetHue = 180; 
       //showColour(0x00ffff);  
      
    } else if(strcmp(msg, "orange") == 0) { 
      targetHue = 60; 
       //showColour(0xffab00);  
      
    }
    
    
    
  }
}



void showStringOnLCD(char * msg) { 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);


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
  String sendstring = String(String("{\"type\" : \"register\", \"id\" : \"")+id+String("\"}"));
  wsclient.send(sendstring);
  lastSend = millis(); 

}


void setPixelHSB(int pixelnum, int hue, int saturation, int brightness) { 
  
  while(hue<0) hue+=360; 
  while(hue>360) hue-=360; 
  H2R_HSBtoRGB( hue, saturation, brightness, &rgb[0]); 
  strip.setPixelColor(pixelnum, strip.Color(rgb[0], rgb[1], rgb[2]));
  
}


