/*

  ButtonLED
  Makes the LED come on when you push the button
  
  Tasks : Make the LED switch off when you push the button 
          Make the LED stay on for at least a second
          Make the LED flash 3 times when you push the button
 */

const int ledPin = 2;
const int buttonPin = 21; 

// the setup routine runs at the start

void setup() {                
  // initialize the digital pins 
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);   
}

// the loop routine runs over and over again forever:
void loop() {
  
  boolean buttonPushed = !digitalRead(buttonPin); // read the button pin
  digitalWrite(ledPin, !buttonPushed);    // set the LED to be on or off
  
}

