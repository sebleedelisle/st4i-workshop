#include <HSBColor.h>

const int redPin = 44; 
const int greenPin = 45; 
const int bluePin = 46; 

const int buttonPin = 21; 
const int potPin = A5; 
float progress = 0; 

int rgb[] = { 0,0,0}; 

void setup() { 
  
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  pinMode(buttonPin, INPUT); 
  pinMode(potPin, INPUT); 

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

}



