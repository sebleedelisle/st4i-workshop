
#include "Adafruit_CC3000.h"
#include "SPI.h"
#include "ST4iWifiManager.h"
#include "LedControl.h"

#include <Time.h>

ST4iWifiManager wifi; 

#define WIFI "68 Middle Street"
#define PASSWORD "thund3rstorm"

const int wifiLed = 2; 
const int photocellPin = A8; 
const int potPin = A5; 

LedControl lc=LedControl(26,30,28,1);

const unsigned long responseTimeout = 15L * 1000L; // Max time to wait for data from server
int countdown       = 0;  // loop() iterations until next time server query
unsigned long lastPolledTime  = 0L, // Last value retrieved from time server
              sketchTime      = 0L; // CPU milliseconds since last server query

Adafruit_CC3000_Client client;

time_t prevDisplay = 0; // when the digital clock was displayed

void setup() { 

  Serial.begin(115200);

  init7SegDisplay();

  // wifi.init(int okLedPin, int brightness (0 to 255, but only works on PWM pins))
  wifi.init(wifiLed, 30); 
  
  // SSID, password, security type
  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 

}

void loop() {
  
  
  if(countdown == 0) {            // Time's up?
    unsigned long t  = getTime(); // Query time server
    if(t) {                       // Success?
      lastPolledTime = t;         // Save time
      sketchTime     = millis();  // Save sketch time of last valid time query
      countdown      = 24*60*4-1; // Reset counter: 24 hours * 15-second intervals
      setTime(t);
    }
  } 
  else {
    countdown--;                  // Don't poll; use math to figure current time
  }

  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
      showTimer();

    }
  }

  int brightness = map(analogRead(potPin),0,1024,0,15); 
  //int brightness = constrain(map(analogRead(photocellPin), 400,800,1,10),1,10); 
  lc.setIntensity(0,brightness);
  
  delay(10L); // Pause 1 second
}


// Minimalist time server query; adapted from Adafruit Gutenbird sketch,
// which in turn has roots in Arduino UdpNTPClient tutorial.
unsigned long getTime(void) {

  uint8_t       buf[48];
  unsigned long ip, startTime, t = 0L;

  Serial.print(F("Locating time server..."));

  // Hostname to IP lookup; use NTP pool (rotates through servers)
  if(wifi.cc3000.getHostByName("pool.ntp.org", &ip)) {
    static const char PROGMEM
      timeReqA[] = { 
      227,  0,  6, 236     }
    ,
    timeReqB[] = {  
      49, 78, 49,  52     };

    Serial.println(F("\r\nAttempting connection..."));
    startTime = millis();
    do {
      client = wifi.cc3000.connectUDP(ip, 123);
    } 
    while((!client.connected()) &&
      ((millis() - startTime) < connectTimeout));

    if(client.connected()) {
      Serial.print(F("connected!\r\nIssuing request..."));

      // Assemble and issue request packet
      memset(buf, 0, sizeof(buf));
      memcpy_P( buf    , timeReqA, sizeof(timeReqA));
      memcpy_P(&buf[12], timeReqB, sizeof(timeReqB));
      client.write(buf, sizeof(buf));

      Serial.print(F("\r\nAwaiting response..."));
      memset(buf, 0, sizeof(buf));
      startTime = millis();
      while((!client.available()) &&
        ((millis() - startTime) < responseTimeout));
      if(client.available()) {
        client.read(buf, sizeof(buf));
        t = (((unsigned long)buf[40] << 24) |
          ((unsigned long)buf[41] << 16) |
          ((unsigned long)buf[42] <<  8) |
          (unsigned long)buf[43]) - 2208988800UL;
        Serial.print(F("OK\r\n"));
      }
      client.close();
    }
  }
  if(!t) Serial.println(F("error"));
  return t;
}


void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void showTimer() { 

  //int cents = ((1414304029*1000) - (millis())) /10 %100; 
  int secs = second(); 
  int mins = minute(); 
  int hours = hour(); 
  //int days = t/60/60/24; 

  int pos = 0; 

  // set cents
  //lc.setDigit(0,pos++, cents%10, false);
  //lc.setDigit(0,pos++, cents/10%10, fals             e);

  // set secs
  lc.setDigit(0,pos++,secs%10, false);
  lc.setDigit(0,pos++,secs/10%10, false);
  pos++;
  // mins
  lc.setDigit(0,pos++,mins%10, false);
  lc.setDigit(0,pos++,mins/10%10, false);
  pos++,
  // hours
  lc.setDigit(0,pos++,hours%10, false);
  lc.setDigit(0,pos++,hours/10%10,false);

  // days
  //  lc.setDigit(0,pos++,days%10, true);
  //  lc.setDigit(0,pos++,days/10%10, false);

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


