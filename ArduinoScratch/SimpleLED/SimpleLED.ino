#include <HSBColor.h>

const int redPin = 44; 
const int greenPin = 45; 
const int bluePin = 46; 

const int buttonPin = 21; 
const int knobPin = A5; 
float progress = 0; 

int rgb[] = {0,0,0}; 

void setup() { 
  Serial.begin(115200); 
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  pinMode(buttonPin, INPUT); 
  pinMode(knobPin, INPUT); 
  
}


void loop() {
  //H2R_HSBtoRGB(int hue, int saturation, int brightness, int* rbg_array);
  int brightness = 10; 
  if(!digitalRead(buttonPin)) brightness = 100; 
  
  H2R_HSBtoRGB( map(analogRead(knobPin),0,1024,0,359), 100, brightness, &rgb[0]); 
  //H2R_HSBtoRGB( 0, 50, map(analogRead(knobPin),0,1024,0,100), &rgb[0]); 
  
  analogWrite(redPin, rgb[0]); 
  analogWrite(greenPin,  rgb[1]); 
  analogWrite(bluePin, rgb[2]);
  
}


