#include <Wire.h>
#include "LedControl.h"

#include "Adafruit_CC3000.h"
#include <SPI.h>
#include "ST4iWifiManager.h"

#define WIFI "LeanDUS"
#define PASSWORD "LeanDUS2015"

#include <JsonParser.h>
#include "utility/debug.h" // for the freemem util
#include <Time.h>

#include "U8glib.h"

uint8_t draw_state = 0;
using namespace ArduinoJson::Parser;
ST4iWifiManager wifi; 

#define BUZZER_PIN 34
#define WIFI_OK_LED 2

// the OLED display object. Horrible name huh!? 

U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NO_ACK); 
char* outputString = "000:00:00:00";
int position[] = {
  0,0};

JsonParser<100> parser;
String jsonString;


long nextPassTime;
long duration; 

void setup() { 
  Serial.begin(115200);
  tone(BUZZER_PIN,500,400); 

  // initialise the LED 7 segment display
  initOLED();

  // initialise the wifi manager and connect to the network
  wifi.init(WIFI_OK_LED, 30); 

  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 


  // first step - where are we? Ask a geoip server!
  // give it your ip address and it tells you your lat / long
  jsonString = wifi.getWebPage("www.telize.com", "/geoip"); 


  Serial.println("--------------------");

  // extract the lat long from the json string that you get back
  char jsonchar[jsonString.length()+1];
  jsonString.toCharArray(jsonchar, jsonString.length()+1);
  JsonObject root = parser.parse(jsonchar);

  if (!root.success())
  {
    Serial.println("JsonParser.parse() failed");

    // probably should fail a bit more gracefully than this...
    return;
  }

  char* latitude = root["latitude"]; 
  Serial.println(latitude); 

  char* longitude = root["longitude"]; 
  Serial.println(longitude); 

  // Open notify tells us the next over pass of the ISS 
  // based on our lat long. 
  // Figure out the URL path that we need, and insert the 
  // lat/long into the query string
  char path[255];
  sprintf(path,"/iss-pass.json?lat=%s&lon=%s&n=2",latitude, longitude);

  Serial.println("opening page : ") ; 
  Serial.print("api.open-notify.org"); 
  Serial.println(path);

  // and get the web page - this function blocks
  jsonString = wifi.getWebPage("api.open-notify.org", path);

  Serial.print("RAM : ");
  Serial.println(getFreeRam(), DEC);
  Serial.println("--------------------");

  jsonString.toCharArray(jsonchar, jsonString.length()+1);
  Serial.println("parsing json");

  Serial.println("------------"); 
  Serial.print(jsonchar); 
  Serial.println("------------"); 

  // parse the json
  JsonObject root2 = parser.parse(jsonchar);

  if (!root2.success())
  {
    Serial.println("JsonParser.parse() failed");
    return;
  }   

  // if it worked, then we can extract the info that we need!
  Serial.println("SUCCESS!"); 
  Serial.println(getFreeRam(), DEC);

  long time = root2["request"]["datetime"]; 
  nextPassTime = root2["response"][0]["risetime"];
  duration =  root2["response"][0]["duration"];

  Serial.println((long)root2["response"][0]["risetime"]); 
  Serial.println(nextPassTime); 
  Serial.println(time); 

  // the open notify api tells us the time too, so set our 
  // internal clock using that! 
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
// if it's finished then load the data again. (TO DO)

void loop() { 
  //  Serial.println(nextPassTime - millis()); 

  time_t timetopass = nextPassTime - now(); 

  showTimer(timetopass); 

  // if it's flying over MAKE SOME NOISE! WOOOO
  while((now()>nextPassTime) && (now()<nextPassTime+duration)) { 
    tone(BUZZER_PIN,500,200); 
    outputString = "00:00:00"; 
    updateScreen(); 
    delay(300); 
    outputString = ""; 
    updateScreen(); 

    delay(300); 

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

  if(days>0) 
    sprintf(outputString, "%01d:%02d:%02d:%02d", days, hours, mins, secs ); 
  else if(hours>0) 
    sprintf(outputString, "%01d:%02d:%02d", hours, mins, secs ); 
  else if(mins>0) 
    sprintf(outputString, "%01d:%02d", mins, secs ); 
  else 
    sprintf(outputString, "%01d", secs ); 

  updateScreen(); 

}


void updateScreen() { 


  // Nice fonts what I like : 
  //u8g.setFont(u8g_font_fub25); // basically, helvetica bold
  //u8g.setFont(u8g_font_helvB24);
  //u8g.setFont(u8g_font_profont29);
  //u8g.setFont(u8g_font_profont29);
  //u8g.setFont(u8g_font_tpssb);

  int textwidth; 
  int textheight = 35; 

  u8g.setFont(u8g_font_fub35n);

  textwidth = u8g.getStrWidth(outputString);
  if(textwidth>128) { 
    u8g.setFont(u8g_font_fub30n);
    textwidth = u8g.getStrWidth(outputString);
    textheight = 30; 

  }
  if(textwidth>128) { 
    u8g.setFont(u8g_font_fub25n);
    textwidth = u8g.getStrWidth(outputString);
    textheight = 25; 

  }
  if(textwidth>128) { 
    u8g.setFont(u8g_font_fub20n);
    textwidth = u8g.getStrWidth(outputString);
    textheight = 20; 

  }
  if(textwidth>128) { 
    u8g.setFont(u8g_font_fub17n);
    textwidth = u8g.getStrWidth(outputString);
    textheight = 20; 

  }
  u8g.setFontPosTop();

  position[0] = (128-textwidth)/2 +  1;
  position[1] = (64 - 35)/2;  

  // picture loop  
  u8g.firstPage();  
  do {
    drawOLED();
  } 
  while( u8g.nextPage() );

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

void drawOLED(void) {

  u8g.drawStr( position[0], position[1], outputString); // only currently accommodates for 3 or 4 digits
}


void initOLED() { 
  // comment out if you want the display reversed
  u8g.setRot180();

}




