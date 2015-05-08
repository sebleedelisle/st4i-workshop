
#include "U8glib.h"

U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NO_ACK); 
uint8_t draw_state = 0;

long cashPence = 0;  
char* cashString = "0000000.00"; // initialise with as many chars as we're likely to need!
int position[] = {
  0,0};

void setup(void) {

  Serial.begin(115200); 
  // flip screen, if required
  u8g.setRot180();


}

void loop(void) {

  updateScreen(); 

  cashPence+=200;
  updateCashString(); 

}


void draw(void) {

  u8g.drawStr( position[0], position[1], cashString); // only currently accommodates for 3 or 4 digits
}


void updateScreen() { 


  // Nice fonts what I like : 
  //u8g.setFont(u8g_font_fub25); // basically, helvetica bold
  //u8g.setFont(u8g_font_helvB24);
  //u8g.setFont(u8g_font_profont29);
  //u8g.setFont(u8g_font_profont29);
  //u8g.setFont(u8g_font_tpssb);

  int textwidth; 
  int textheight = 35; 

  u8g.setFont(u8g_font_fub35n);

  textwidth = u8g.getStrWidth(cashString);
  if(textwidth>128) { 
    u8g.setFont(u8g_font_fub30n);
    textwidth = u8g.getStrWidth(cashString);
    textheight = 30; 

  }
  if(textwidth>128) { 
    u8g.setFont(u8g_font_fub25n);
    textwidth = u8g.getStrWidth(cashString);
    textheight = 25; 

  }
  if(textwidth>128) { 
    u8g.setFont(u8g_font_fub20n);
    textwidth = u8g.getStrWidth(cashString);
    textheight = 20; 

  }
  u8g.setFontPosTop();

  position[0] = (128-textwidth)/2 +  1;
  position[1] = (64 - 35)/2;  

  // picture loop  
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );

}
void updateCashString() { 
  
  // have to convert to int to avoid stupid annoying issues with sprintf and long
  int cashpence = (int)(cashPence %100); // int can't be more than 30000 odd 
  int cashpounds = (int)(cashPence/100);
  
  if((cashPence>-100) && (cashPence<0)) sprintf(cashString, "-0.%02d", abs(cashpence) ); // add a minus sign if necessary
  else sprintf(cashString, "%01d.%02d", (cashpounds), abs(cashpence) ); 
  
 // sprintf(cashString, "%l", cashPence ); 
  
  Serial.println(cashString); 


}




