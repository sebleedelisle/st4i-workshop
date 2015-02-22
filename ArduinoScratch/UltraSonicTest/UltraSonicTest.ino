#include <NewPing.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(19,18,17,16,15,14);

#define TRIGGER_PIN  A11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A12 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


void setup() {
  pinMode(A10, OUTPUT); 
  pinMode(A13, OUTPUT); 
  digitalWrite(A10, HIGH); 
  digitalWrite(A13, LOW); 

  lcd.begin(16, 2);


  lcd.clear();
 

  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");


  lcd.setCursor(0,1);
  
  int numChars; 
  
  if(uS==0) numChars = 16; 
  else numChars = ((float)(uS / US_ROUNDTRIP_CM) / 100.0f * 16.0f);
  //lcd.print(uS / US_ROUNDTRIP_CM);
  for(int i=0; i< 16;i++) { 
    lcd.write( (i<=numChars) ? byte(0xFF) : 32);
  }
  
   lcd.setCursor(0,0);
  lcd.print("Distance : " + String(uS / US_ROUNDTRIP_CM) + "      ");
}

