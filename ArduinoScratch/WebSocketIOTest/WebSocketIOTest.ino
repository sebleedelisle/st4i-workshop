#include <SocketIOClient.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"


#define WLAN_SSID       "LeanDUS"           // cannot be longer than 32 characters!
#define WLAN_PASS       "LeanDUS2015"
#define WLAN_SECURITY   WLAN_SEC_WPA2
// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2); // you can change this clock speed

SocketIOClient client;

void setup() {
  Serial.begin(115200);
  InitializeCC30000();
  //  client.setDataArrivedDelegate(ondata);
  if (!client.connect(cc3000,"10.0.1.32",3000)) Serial.println(F("Not connected."));
}

void loop() {
  client.monitor(cc3000);
  client.sendMessage("ping");

  delay(2000);
}

void InitializeCC30000(void)
{
  // Initialise the module
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }


  Serial.print(F("\nAttempting to connect to ")); 
  Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  Serial.println(F("Connected!"));
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  
}
////
//void dataArrived(WebSocketClient client, char *data) {
void dataArrived(SocketIOClient client, char *data) {
  Serial.println("Data Arrived");
}



