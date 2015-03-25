#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
//#include "DHT.h"
#include <stdlib.h>
//#include <Wire.h>
#include <SocketIOClient.h>

#define ADAFRUIT_CC3000_IRQ 3 // MUST be an interrupt pin!
#define ADAFRUIT_CC3000_VBAT 5
#define ADAFRUIT_CC3000_CS 10

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2); 
SocketIOClient client;

#define WLAN_SSID "indi-wifi" 
#define WLAN_PASS "id1w679mv"
#define WLAN_SECURITY WLAN_SEC_WPA2

void setup() { 
  Serial.begin(115200);

  if (!cc3000.begin())
  {
    Serial.print(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }

  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.print(F("Failed to connect!"));
    while(1);
  }

  Serial.print(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  } 

  Serial.println(F("Connecting to server."));
  client.connect(cc3000, "ws://echo.websocket.org/", 80);
  Serial.println("Connected");
  client.setDataArrivedDelegate(dataArrived);

}

void loop() { 
  client.monitor(cc3000);
  client.sendMessage("Hello world!");
  delay(2000);
}

void dataArrived(SocketIOClient client, char *data) { 
  Serial.println("Data Arrived: ");
  Serial.println( data);
}


