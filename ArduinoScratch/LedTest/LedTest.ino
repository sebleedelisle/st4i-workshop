const int netLedPin = 17;
const int serverLedPin = 18; 
const int bigLedPin = 19; 
const int buttonPin = 20; 



void setup() { 
  pinMode(netLedPin, OUTPUT); 
  pinMode(serverLedPin, OUTPUT); 
  pinMode(bigLedPin, OUTPUT); 
  pinMode(buttonPin, INPUT_PULLUP); 
  
  digitalWrite(netLedPin, HIGH); 
  digitalWrite(serverLedPin, HIGH); 
  digitalWrite(bigLedPin, HIGH); 
  
  
}

void loop() {
  digitalWrite(bigLedPin, digitalRead(buttonPin)); 
  
}
