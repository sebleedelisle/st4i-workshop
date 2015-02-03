/*

 ButtonLED
 Makes the LED come off when you push the button
 
 Tasks : Make the LED switch on when you push the button 
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
  if(buttonPushed) { 
    digitalWrite(ledPin, HIGH);    // turn the LED on
    while(!digitalRead(buttonPin)); // wait til the button is released
    delay(1000); // wait a second
  } 
  else { 
    digitalWrite(ledPin, LOW); 
  }

}


