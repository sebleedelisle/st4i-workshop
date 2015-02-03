#include "Adafruit_CC3000.h"
#include "SPI.h"
#include "WifiManager.h"

WifiManager wifi; 

#define WIFI "68 Middle Street"
#define PASSWORD "thund3rstorm"

const int wifiLed = 2; 

void setup() { 
  
  Serial.begin(115200);
  // wifi.init(int okLedPin, int brightness (0 to 255, but only works on PWM pins))
  wifi.init(wifiLed, 30); 
  // SSID, password, security type
  wifi.connect(WIFI, PASSWORD, WLAN_SEC_WPA2); 

  Serial.println(wifi.getWebPage("www.telize.com", "/geoip")); 

}

void loop() { 



}

