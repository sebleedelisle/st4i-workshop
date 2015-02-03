#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"

int printer_RX_Pin = 18;  // This is the green wire
int printer_TX_Pin = 19;  // This is the yellow wire

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

void setup(){
  Serial.begin(115200);
  Serial.println("hello");
  pinMode(17, OUTPUT); 
  digitalWrite(17, LOW); // To also work w/IoTP printer
  printer.begin();


  printer.println("Hello World!"); 
  printer.println("0123456789");

  printer.sleep();      // Tell printer to sleep
  printer.wake();       // MUST call wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults
}

void loop() {
}

