
const int ledPin = 2; 
const int potPin = A5; 


void setup() { 
  
  pinMode(ledPin, OUTPUT); 
  pinMode(potPin, INPUT); 
}


void loop() {
  int brightness =  analogRead(potPin)/4; 
  analogWrite(ledPin, brightness); 
}



