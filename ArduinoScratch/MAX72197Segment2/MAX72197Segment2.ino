//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(26,30,28,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15);
  /* and clear the display */
  lc.clearDisplay(0);
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.setChar(0,6,'a',false);
  lc.setRow(0,5,0x05);
  lc.setChar(0,4,'d',false);
  lc.setRow(0,3,0x1c);
  lc.setRow(0,2,B00010000);
  lc.setRow(0,1,0x15);
  lc.setRow(0,0,0x1D);

  delay(delaytime*10);
  lc.clearDisplay(0);

  lc.setChar(0,7,'-', false);
  lc.setChar(0,6,'-', false);

  lc.setRow(0,5,B01011011);
  lc.setRow(0,4,B00001111);
  lc.setDigit(0,3,4, false);
  lc.setDigit(0,2,1, false);
  
  lc.setChar(0,1,'-', false);
  lc.setChar(0,0,'-', false);

  delay(delaytime*10);
  lc.clearDisplay(0);

} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
  for(int i=0;i<13;i++) {
    lc.setDigit(0,7,i,false);
    lc.setDigit(0,6,i+1,false);
    lc.setDigit(0,5,i+2,false);
    lc.setDigit(0,4,i+3,false);
    lc.setDigit(0,3,i,false);
    lc.setDigit(0,2,i+1,false);
    lc.setDigit(0,1,i+2,false);
    lc.setDigit(0,0,i+3,false);
    delay(delaytime);
  }
  lc.clearDisplay(0);
  delay(delaytime);
}

void loop() { 
  writeArduinoOn7Segment();
  scrollDigits();
}
