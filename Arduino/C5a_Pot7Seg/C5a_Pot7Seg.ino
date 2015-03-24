#include <LedControl.h>
#include <HSBColor.h>

// LedControl is the library for running the 
// 7 segment LED display. Set it up with the 
// pins for DIN, CLK and LOAD/CS
LedControl lc=LedControl(26,30,28,1);

const int redPin = 44; 
const int greenPin = 45; 
const int bluePin = 46; 

const int buttonPin = 21; 
const int potPin = A5; 

int rgb[] = { 0,0,0}; 


void setup() { 
  
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  pinMode(buttonPin, INPUT); 
  pinMode(potPin, INPUT); 

  init7SegDisplay();


}


void loop() {
  int brightness = 10; 
  
  if(!digitalRead(buttonPin)) brightness = 100; 
  // hue = 0 to 360, brightness = 0 to 100, sat 0 to 100
  //H2R_HSBtoRGB(int hue, int saturation, int brightness, int* rbg_array);
  int hue = map(analogRead(potPin),0,1024,0,360);
  
  H2R_HSBtoRGB(hue , 100, brightness, &rgb[0]); 

  analogWrite(redPin, rgb[0]); 
  analogWrite(greenPin,  rgb[1]); 
  analogWrite(bluePin, rgb[2]);
  
  showNumber(analogRead(potPin));

}




void init7SegDisplay() { 

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

long pow10[] = {1,10,100,1000,10000,100000,1000000,10000000};

void showNumber(int32_t number) {
  
  lc.clearDisplay(0); 
  
  boolean negative = false; 
  
 
  if(number<0) { 
    number = abs(number); 
    negative = true; 
    
  }
  
  for(int digit = 0; digit<8; digit++) { 
    unsigned long power= pow10[digit]; //pow(10,digit); 
    if((digit>0) && (number < power)) {
      if(negative) lc.setChar(0,digit,'-',false);
      break;
    }
     lc.setDigit(0,digit,(number/power)%10,false);
  }

}


