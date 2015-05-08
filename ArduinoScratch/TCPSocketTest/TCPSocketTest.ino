
/*************************************************** 
 * This is an example for the Adafruit CC3000 Wifi Breakout & Shield
 * 
 * Designed specifically to work with the Adafruit WiFi products:
 * ----> https://www.adafruit.com/products/1469
 * 
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 * 
 * Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
 * BSD license, all text above must be included in any redistribution
 ****************************************************/

/*
This example does a test of the TCP client capability:
 * Initialization
 * Optional: SSID scan
 * AP connection
 * DHCP printout
 * DNS lookup
 * Optional: Ping
 * Connect to website and print out webpage contents
 * Disconnect
 SmartConfig is still beta and kind of works but is not fully vetted!
 It might not work on all networks!
 */
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

#define WLAN_SSID       "LeanDUS"           // cannot be longer than 32 characters!
#define WLAN_PASS       "LeanDUS2015"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define IDLE_TIMEOUT_MS  60000      // Amount of time to wait (in milliseconds) with no data 
// received before closing the connection.  If you know the server
// you're accessing is quick to respond, you can reduce this value.
#define CRLF "\r\n"
// What page to grab!
#define WEBSITE      "10.0.1.32"
#define PATH         "/"
#define PORT         8000
//#define WEBPAGE      "/testwifi/index.html"

#define RED     0x110000
#define ORANGE  0x118800
#define GREEN   0x001100
#define BLACK   0x000000


/**************************************************************************/
/*!
 @brief  Sets up the HW and the CC3000 module (called automatically
 on startup)
 */
/**************************************************************************/

uint32_t ip;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(2, A14, NEO_GRB + NEO_KHZ800);

Adafruit_CC3000_Client client;


String msg; 

void setup(void)
{

  initLEDs(); 

  Serial.begin(115200);

  initCC3000();
  connectToWifi(); 
  connectToSocketServer(WEBSITE, PORT); 



  /* Read data until either the connection is closed, or the idle timeout is reached. */
  unsigned long lastRead = millis();



  //
  //  client.close();
  //  Serial.println(F("-------------------------------------"));
  //
  //  socketRedLight();
  //
  //  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  //  /* the next time your try to connect ... */
  //  Serial.println(F("\n\nDisconnecting"));
  //  cc3000.disconnect();

}

void loop(void)
{
  if (client.connected()) { // && (millis() - lastRead < IDLE_TIMEOUT_MS)) {

    if(client.available()) socketLightOff(); 
    else socketGreenLight();
    
    msg = ""; 
    
    while (client.available()) {
      char c = client.read();
      //Serial.print(int(c)); 
      //Serial.print(":");
      //Serial.print(c);
      //lastRead = millis();
      
      msg+=c;
      
    }
    if(msg.length()>0) Serial.println(msg);
    //delay(100); 
  } else { 
    // reconnect 
    socketRedLight();
    connectToSocketServer(WEBSITE, PORT); 
  }
}


/**************************************************************************/
/*!
 @brief  Begins an SSID scan and prints out all the visible networks
 */
/**************************************************************************/

void listSSIDResults(void)
{
  uint32_t index;
  uint8_t valid, rssi, sec;
  char ssidname[33]; 

  if (!cc3000.startSSIDscan(&index)) {
    Serial.println(F("SSID scan failed!"));
    return;
  }

  Serial.print(F("Networks found: ")); 
  Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);

    Serial.print(F("SSID Name    : ")); 
    Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}




