
// (Based on Ethernet's WebClient Example)

#include <SPI.h>
#include <SC16IS750.h>
#include <WiFly.h>

WiFlyClient client = WiFlyClient();

void setup() {
  
  Serial.begin(9600);
  
  SC16IS750.begin();
  
  WiFly.setUart(&SC16IS750);
  WiFly.begin();
  
  WiFly.sendCommand(F("set wlan auth 0"));
  WiFly.sendCommand(F("set wlan channel 0"));
  WiFly.sendCommand(F("set ip dhcp 1"));
  
  Serial.println(F("Setting things up..."));
  
  if (!WiFly.sendCommand(F("join automata_arduino"), "Associated!", 20000, false)) {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }
  
  if (!WiFly.waitForResponse("DHCP in", 10000)) {
    Serial.println("DHCP failed.");
    while (1) {
      // Hang on failure.
    }
  }
  
  Serial.println("connecting...");
  
  if (client.connect("google.com", 80)) {
    Serial.println("connected");
    client.println(F("GET /search?q=arduino HTTP/1.0"));
    client.println();
  } else {
    Serial.println("connection failed");
  }
  
}

void loop() {
  while (client.available()) {
    int i = client.read();
    
    if (i > -1) {
      Serial.print((char) i);
    }
  }
  
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
}


