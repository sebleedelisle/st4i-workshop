
const int ledPin = 2; 
const int potPin = A5; 


void setup() { 
  
  pinMode(ledPin, OUTPUT); 
  pinMode(potPin, INPUT); 
}


void loop() {
  int brightness = map( analogRead(potPin), 0, 1023,0,255); 
  analogWrite(ledPin, brightness); 
}



