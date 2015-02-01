
const int wifiLed = 2; 

void initLEDs() {
  // init pixel pins
  pinMode(wifiLed, OUTPUT); 
  //pinMode(15, OUTPUT); 
  digitalWrite(wifiLed, LOW); 
  //digitalWrite(15, HIGH); 
  
 

}
//
//void flashLed(int count, uint16_t colour, int pixelNum) { 
//
//  boolean forever = count ==0; 
//  while(forever | (count--)) { 
//    leds.setPixelColor(pixelNum, 0x00000); 
//    leds.show(); 
//    delay(100); 
//    leds.setPixelColor(pixelNum, colour); 
//    delay(100); 
//    leds.show();
//    
//  }  
//
//}
//
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
