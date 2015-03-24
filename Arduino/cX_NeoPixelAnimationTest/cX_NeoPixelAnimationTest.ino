#include <Adafruit_NeoPixel.h>
#include <HSBColor.h>

int numPixels = 12; 
int rgb[] = {0,0,0}; 

Adafruit_NeoPixel strip(numPixels, 22,  NEO_GRB + NEO_KHZ800); 

void setup() { 
 Serial.begin(115200); 
 strip.begin();  
  
}



void loop() { 
  
  int hue = 13; 
  int saturation = 100; 
  //int brightness = 100; 
  
  
  for(int i = 0; i<numPixels; i++) { 
    int brightness = map(sin((double)(millis()+(i*100))*0.01f ) * 100, -100, 100, 0,100);
    setPixelHSB(i, hue, saturation, brightness); 
  } 
  strip.show(); 
  
}

void setPixelHSB(int pixelnum, int hue, int saturation, int brightness) { 
  
  while(hue<0) hue+=360; 
  while(hue>360) hue-=360; 
  H2R_HSBtoRGB( hue, saturation, brightness, &rgb[0]); 
  strip.setPixelColor(pixelnum, strip.Color(rgb[0], rgb[1], rgb[2]));
  
}

