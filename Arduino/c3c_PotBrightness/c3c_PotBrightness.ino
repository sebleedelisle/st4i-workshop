
const int ledPin = 2; 
const int potPin = A5; 


void setup() { 
  
  pinMode(ledPin, OUTPUT); 
  pinMode(potPin, INPUT); 
}


void loop() {
  int speed =  analogRead(potPin); 
  digitalWrite(ledPin, HIGH);
  delay(speed);  
  digitalWrite(ledPin, LOW); 
  delay(speed); 
  
}



