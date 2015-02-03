/*

 ButtonLED
 Makes the LED stay on for a second - non-blocking code version!
 
 If you can make your code non-blocking it's always better, especially if you're 
 also reading and writing data to the internet.

 */

const int ledPin = 2;
const int buttonPin = 21; 

unsigned long lastButtonPushTime = 0; 

// the setup routine runs at the start

void setup() {                
  // initialize the digital pins 
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);   
}

// the loop routine runs over and over again forever:
void loop() {

  boolean buttonPushed = !digitalRead(buttonPin); // read the button pin

  if(buttonPushed) { 
    digitalWrite(ledPin, HIGH);    // turn the LED on
    lastButtonPushTime = millis(); // set the last time the button was pressed to now
  } 
  
  // if it's more than a second since we last pressed the button, then 
  // turn the LED off.
  
  // Note the typing to unsigned long in the if statement - that gets around 
  // the fact that millis() resets to zero after 9.5 hours
  
  if((unsigned long) (millis()-lastButtonPushTime) > 1000) { 
     digitalWrite(ledPin, LOW);  
  }

}


