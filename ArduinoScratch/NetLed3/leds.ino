
const int wifiLed = 2; 

void initLEDs() {
  // init pixel pins
  pinMode(wifiLed, OUTPUT); 
  //pinMode(15, OUTPUT); 
  digitalWrite(wifiLed, LOW); 
  //digitalWrite(15, HIGH); 
  
 flashLed(wifiLed,5);

}

void flashLed(int pinNum, int count) { 

  boolean forever = count ==0; 
  while(forever | (count--)) { 
    digitalWrite(pinNum, HIGH); 
    delay(100); 
    digitalWrite(pinNum, LOW); 
    delay(100); 
    
  }  

}

//
//void wifiRedLight() { 
////  leds.setPixelColor(0, RED); 
////  leds.show(); 
//
//}
//
//
//void wifiGreenLight() { 
////  leds.setPixelColor(0, GREEN); 
////  leds.show(); 
//
//
//}
//void wifiOrangeLight() { 
////  leds.setPixelColor(0, ORANGE); 
////  leds.show(); 
//
//}
//
//void socketRedLight(){ 
////  leds.setPixelColor(1, RED); 
////  leds.show(); 
//} 
//
//void socketOrangeLight(){ 
////  leds.setPixelColor(1, ORANGE); 
////  leds.show(); 
//}
//
//void socketGreenLight(){ 
////  leds.setPixelColor(1, GREEN); 
////  leds.show(); 
//}
//void socketLightOff(){ 
////  leds.setPixelColor(1, BLACK); 
////  leds.show(); 
//}
