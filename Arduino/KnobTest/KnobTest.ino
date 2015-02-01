#include <NewPing.h>

int knobPin = A5; 
int photocellPin = A8; 
int redPin = 44; 
int greenPin = 45; 

NewPing sonar(A13, A14, 100); 


void setup() { 

  pinMode(A12, OUTPUT); 
  pinMode(A15, OUTPUT); 
  digitalWrite(A12, HIGH); 
  digitalWrite(A15, LOW); 
  
  pinMode(redPin, OUTPUT); 
  pinMode(knobPin, INPUT); 
  pinMode(photocellPin, INPUT); 
  Serial.begin(115200);   

}

void loop() { 

  int pingTime = sonar.ping(); 
  
  int knobLevel = analogRead(knobPin); 
  int photoLevel = analogRead(photocellPin); 

  analogWrite(redPin, map(knobLevel, 0, 1023, 0, 255));
  analogWrite(greenPin, constrain(map(photoLevel, 300, 800, 0, 255), 0, 255));

  Serial.println(pingTime); 
  delay(50);  

}



