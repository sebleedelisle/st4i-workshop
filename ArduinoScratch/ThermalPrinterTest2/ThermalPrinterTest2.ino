int heatTime = 255; //80 is default from page 23 of datasheet. Controls speed of printing and darkness
int heatInterval = 255; //2 is default from page 23 of datasheet. Controls speed of printing and darkness
char printDensity = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.
char printBreakTime = 15; //Not sure what the defaut is. Testing shows the max helps darken text. From page 23.


void setup() { 

   pinMode(17, OUTPUT); digitalWrite(17, LOW); // To also work w/IoTP printer
 
  Serial.begin(115200);

  Serial1.begin(9600); 


  Serial1.write(255); 
   for(uint8_t i=0; i<10; i++) {
    Serial1.write(27);
    delay(100);
  }
  
   Serial1.write(27);
   Serial1.write(64);

  Serial1.write(27);
  Serial1.write(55);
  Serial1.write(20); //Default 64 dots = 8*('7'+1)
  Serial1.write(heatTime); //Default 80 or 800us
  Serial1.write(heatInterval); //Default 2 or 20us

  //Modify the print density and timeout
  Serial1.write(18);
  Serial1.write(35);
  int printSetting = (printDensity<<4) | printBreakTime;
  Serial1.write(printSetting); //Combination of printDensity and printBreakTime

  delay(5000);

  Serial1.println("Hello World!");
  Serial1.println("12345678901234567890123456789012");
  Serial1.write(10); //Sends the LF to the printer, advances the paper
  Serial1.write(10);


Serial.print("FINISHED"); 




}


void loop() {





}



