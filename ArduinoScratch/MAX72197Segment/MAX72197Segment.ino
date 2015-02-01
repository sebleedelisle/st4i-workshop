//We always have to include the library
#include "LedControl.h"
#include <Time.h>  

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 DataIn 
 CLK 
 CS/LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(26,30,28,1);
  
/* we always wait a bit between updates of the display */
unsigned long delaytime=25;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,5);
  /* and clear the display */
  lc.clearDisplay(0);
}


void loop() { 
    showTime(1414304029 - (millis()/1000)); 
  
   //lc.clearDisplay(0);
  
  // lc.setDigit(0,3,i,false);
  
  
  
}


void showTime(time_t t) { 
    
   int cents = ((1414304029*1000) - (millis())) /10 %100; 
   int secs = second(t); 
   int mins = minute(t); 
   int hours = hour(t); 
   int days = t/60/60/24; 
   
   int pos = 0; 
   
     // set cents
   lc.setDigit(0,pos++, cents%10, false);
   lc.setDigit(0,pos++, cents/10%10, false);

   // set secs
   lc.setDigit(0,pos++,secs%10, true);
   lc.setDigit(0,pos++,secs/10%10, false);
   
   // mins
   lc.setDigit(0,pos++,mins%10, true);
   lc.setDigit(0,pos++,mins/10%10, false);
   
   
   // hours
   lc.setDigit(0,pos++,hours%10, true);
   lc.setDigit(0,pos++,hours/10%10, false);
   
    // hours
   lc.setDigit(0,pos++,days%10, true);
   lc.setDigit(0,pos++,days/10%10, false);
  
  
}
