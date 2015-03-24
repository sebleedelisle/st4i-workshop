//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 Connect VCC to 5v, GND to ground. 
 
 **********
 pin 26 is connected to the DataIn 
 pin 28 is connected to the CS / LOAD 
 pin 30 is connected to CLK 
 We have only a single MAX72XX.
 
 */
LedControl lc=LedControl(26,30,28,1);
// brightness = 0 to 15
const int brightness =15; 
/* we always wait a bit between updates of the display */
unsigned long delaytime=100;

float angle = 0; 
float speed = 0.1; 


void setup() {
  Serial.begin(115200);

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,brightness);
  /* and clear the display */
  lc.clearDisplay(0);
}



void loop() {
  
      speed = (sin((float)millis()*0.0001f)) *0.1;
      angle += speed; //  (PI*2.0f) * (float)(i/360.0f); 
      
      float x = cos(angle)*3.5; 
      float y = sin(angle)*3.5; 
//      Serial.print(angle); 
//      Serial.print(","); 
//      Serial.print(x); 
//      Serial.print(","); 
//      Serial.println(y); 

      lc.setLed(0,x+4,y+4,true);
      lc.setLed(0,4-x,4-y,true);
      delay(delaytime/40);
      lc.setLed(0,x+4,y+4,false);
      lc.setLed(0,4-x,4-y,false);
  
  //writeArduinoOnMatrix();
 // spin(); 
//  rows();
//  columns();
//  single();
//  single();
//  single();
}

void spin() { 
  //for(float j=0; j<3.5; j+=0.5) { 
    for(int i = 0; i<360; i+=3) { 


    }  
 //}


}


/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[5]={
    B01111110,B10001000,B10001000,B10001000,B01111110  };
  byte r[5]={
    B00111110,B00010000,B00100000,B00100000,B00010000  };
  byte d[5]={
    B00011100,B00100010,B00100010,B00010010,B11111110  };
  byte u[5]={
    B00111100,B00000010,B00000010,B00000100,B00111110  };
  byte i[5]={
    B00000000,B00100010,B10111110,B00000010,B00000000  };
  byte n[5]={
    B00111110,B00010000,B00100000,B00100000,B00011110  };
  byte o[5]={
    B00011100,B00100010,B00100010,B00100010,B00011100  };

  /* now display them one by one with a small delay */
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  delay(delaytime);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  delay(delaytime);
  lc.setRow(0,0,d[0]);
  lc.setRow(0,1,d[1]);
  lc.setRow(0,2,d[2]);
  lc.setRow(0,3,d[3]);
  lc.setRow(0,4,d[4]);
  delay(delaytime);
  lc.setRow(0,0,u[0]);
  lc.setRow(0,1,u[1]);
  lc.setRow(0,2,u[2]);
  lc.setRow(0,3,u[3]);
  lc.setRow(0,4,u[4]);
  delay(delaytime);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  delay(delaytime);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  delay(delaytime);
  lc.setRow(0,0,o[0]);
  lc.setRow(0,1,o[1]);
  lc.setRow(0,2,o[2]);
  lc.setRow(0,3,o[3]);
  lc.setRow(0,4,o[4]);
  delay(delaytime);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
  delay(delaytime);
}

/*
  This function lights up a some Leds in a row.
 The pattern will be repeated on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(delaytime);
    lc.setRow(0,row,B11111111);
    delay(delaytime);
    lc.setRow(0,row,(byte)0);
    //for(int i=0;i<row;i++) {
    //      delay(delaytime);
    //      lc.setRow(0,row,B11111111);
    //      delay(delaytime);
    //      lc.setRow(0,row,(byte)0);
    //}
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(delaytime);
    lc.setColumn(0,col,B11111111);
    delay(delaytime);
    lc.setColumn(0,col,(byte)0);

  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void single() {
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime/10);
      lc.setLed(0,row,col,true);
      delay(delaytime/10);
      lc.setLed(0,row,col,false);
    }
  }
}


