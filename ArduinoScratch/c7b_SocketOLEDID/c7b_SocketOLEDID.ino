
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "ST4iWifiManager.h"
#include <WebSocketClient.h>
#include <JsonParser.h>
using namespace ArduinoJson::Parser;

#include <U8glib.h>

//OLED SCL pin 21, SDA pin 22. 
U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NO_ACK); 


ST4iWifiManager wifi; 

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

int count = 0;

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
    wsclient.send(String("{\"type\" : \"heartbeat\", \"id\" : \"")+id+String("\"}"));
    lastSend = millis();
   
  }


  if(!digitalRead(buttonPin) != buttonPushed) { 
    buttonPushed = !buttonPushed; 
    String msg; 

    if(buttonPushed) 
      msg = String("{\"type\" : \"light\", \"data\" : 1, \"id\" : \"")+id+String("\"}");
    else 
      msg = String("{\"type\" : \"light\", \"data\" : 0, \"id\" : \"")+id+String("\"}");
    Serial.print("sending : "); 
    Serial.println(msg); 

    wsclient.send(msg);
    Serial.println("sent");
  }
  
  updateScreen();

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
      count++; 
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
  String sendstring = String(String("{\"type\" : \"register\", \"id\" : \"")+id+String("\"}"));
  wsclient.send(sendstring);
  lastSend = millis(); 

}



void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22, "Hello World!");
  u8g.drawStr(0, 32, String(count).c_str()); 
  
}



void updateScreen() { 


  // picture loop  
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );

}

