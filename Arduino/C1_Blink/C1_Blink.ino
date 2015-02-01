/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly
 
 Tasks : Make the LED flash faster. 
         AnalogWrite for pulsing the LED
 */

int ledPin = 2;

// the setup routine runs at the start

void setup() {                
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {

  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level) 
  delay(1000); // wait 1000 miliseconds (1 second)
  
  digitalWrite(ledPin, LOW); 
  delay(1000); 

}

