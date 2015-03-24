#include <ServoTimer2.h>


ServoTimer2 servo;

void setup() {
  // put your setup code here, to run once:
  servo.attach(9); 
}

void loop() {
  
  
  servo.write(500); 
  
  delay(1000); 
  servo.write(2000); 
  delay(1000); 
//  
//  // put your main code here, to run repeatedly:
//  for(int pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
//  {                                  // in steps of 1 degree 
//    servo.write(pos);              // tell servo to go to position in variable 'pos' 
//    delay(5);                       // waits 15ms for the servo to reach the position 
//  } 
//  for(int pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
//  {                                
//    servo.write(pos);              // tell servo to go to position in variable 'pos' 
//    delay(5);                       // waits 15ms for the servo to reach the position 
//  } 

}
