


#include <LiquidCrystal.h>

LiquidCrystal lcd(19,18,17,16,15,14);



void setup() {
  pinMode(A10, OUTPUT); 
  pinMode(A13, OUTPUT); 
  digitalWrite(A10, HIGH); 
  digitalWrite(A13, LOW); 

  lcd.begin(16, 2);


  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance : ");

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
}

