#include <Wire.h>
#include "LedControl.h"

#include "Adafruit_CC3000.h"
#include <SPI.h>
#include "WifiManager.h"


#include <JsonParser.h>
#include "utility/debug.h" // for the freemem util
#include <Time.h>

using namespace ArduinoJson::Parser;
WifiManager wifi; 

#define BUZZER_PIN 34
#define WIFI_OK_LED 2

LedControl lc=LedControl(26,30,28,1);

JsonParser<100> parser;
String jsonString;

//long long timeOffset; 
long nextPassTime;
long duration; 

void setup() { 
  Serial.begin(115200);
  tone(BUZZER_PIN,500,400); 

  // initialise the LED 7 segment display
  init7SegDisplay();

  // initialise the wifi manager and connect to the network
  wifi.init(WIFI_OK_LED, 30); 
  
  wifi.connect("Sebs", "Internet", WLAN_SEC_WPA2); 

  
  jsonString = wifi.getWebPage("www.telize.com", "/geoip"); 


  Serial.println("--------------------");

  char jsonchar[jsonString.length()+1];
  jsonString.toCharArray(jsonchar, jsonString.length()+1);
  JsonObject root = parser.parse(jsonchar);

  if (!root.success())
  {
    Serial.println("JsonParser.parse() failed");
    return;
  }
  char* latitude = root["latitude"]; 
  Serial.println(latitude); 

  char* longitude = root["longitude"]; 
  Serial.println(longitude); 

  char path[255];
  sprintf(path,"/iss-pass.json?lat=%s&lon=%s&n=2",latitude, longitude);

  Serial.println("opening page : ") ; 
  Serial.print("api.open-notify.org"); 
  Serial.println(path);


  jsonString = wifi.getWebPage("api.open-notify.org", path);

  ///pos = jsonString.indexOf("\r\n\r\n");
  //jsonString = jsonString.substring(pos+4);
  Serial.print("RAM : ");
  Serial.println(getFreeRam(), DEC);
  Serial.println("--------------------");

  //char jsonchar2[jsonString.length()+1];
  jsonString.toCharArray(jsonchar, jsonString.length()+1);
  Serial.println("parsing json");

  Serial.println("------------"); 
  Serial.print(jsonchar); 
  Serial.println("------------"); 
  JsonObject root2 = parser.parse(jsonchar);

  if (!root2.success())
  {
    Serial.println("JsonParser.parse() failed");
    return;
  }   

  Serial.println("SUCCESS!"); 
  Serial.println(getFreeRam(), DEC);

  long time = root2["request"]["datetime"]; 
  nextPassTime = root2["response"][0]["risetime"];
  duration =  root2["response"][0]["duration"];

  Serial.println((long)root2["response"][0]["risetime"]); 
  Serial.println(nextPassTime); 
  Serial.println(time); 



  setTime(time); // should probably subtract latency

  Serial.println("NOW"); 
  printTime(now()); 
  Serial.println("NEXT PASS"); 
  printTime(nextPassTime); 
  Serial.println("DURATION"); 
  Serial.println(duration); 
  Serial.println("PASS #2"); 
  printTime((long)root2["response"][1]["risetime"]); 
}

// get current location from IP address
// get next ISS pass time and duration and current time

// loop 
// calculate time until next pass
// convert it to days / hours / mins / secs / cents
// if it's less than 1 min make alarm
// if it's now then turn on LED strip and countdown pass time
// if it's finished then 

void loop() { 
  //  Serial.println(nextPassTime - millis()); 


  time_t timetopass = nextPassTime - now(); 

  //digitalClockDisplay(nextPassTime); 
  //digitalClockDisplay(now()); 
  //printHoursMinsSecs(timetopass); 

  showTimer(timetopass); 
  // Serial.println(timetopass);
  //delay(1000); 

  while((now()>nextPassTime) && (now()<nextPassTime+duration)) { 
    tone(BUZZER_PIN,500,200); 
    delay(600); 

  }

}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void showTimer(time_t t) { 

  //int cents = ((1414304029*1000) - (millis())) /10 %100; 
  int secs = t%60; 
  int mins = (t/60)%60; 
  int hours = (t/60/60)%60; 
  int days = t/60/60/24; 

  int pos = 0; 

  // set cents
  //lc.setDigit(0,pos++, cents%10, false);
  //lc.setDigit(0,pos++, cents/10%10, fals             e);

  // set secs
  lc.setDigit(0,pos++,secs%10, true);
  lc.setDigit(0,pos++,secs/10%10, false);

  // mins
  lc.setDigit(0,pos++,mins%10, true);
  lc.setDigit(0,pos++,mins/10%10, false);

  // hours
  lc.setDigit(0,pos++,hours%10, true);
  lc.setDigit(0,pos++,hours/10%10, false);

  // days
  lc.setDigit(0,pos++,days%10, true);
  lc.setDigit(0,pos++,days/10%10, false);

}


void printTime(time_t t) { 

  int secs = second(t); 
  int mins = minute(t); 
  int hours = hour(t); 
  int days = day(t); 
  int months = month(t);

  Serial.print(days); 
  Serial.print("/");
  Serial.print(months); 
  Serial.print(" "); 

  if(hours<10) Serial.print(0); 
  Serial.print(hours); 
  Serial.print(":"); 
  if(mins<10) Serial.print(0); 
  Serial.print(mins); 
  Serial.print(":"); 
  if(secs<10) Serial.print(0); 
  Serial.println(secs); 



}

void init7SegDisplay() { 

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);

}


