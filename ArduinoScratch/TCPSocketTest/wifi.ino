



void initCC3000() { 
   
  Serial.println(F("Hello, CC3000!\n")); 
  //Serial.print("Free RAM: "); 
  //Serial.println(getFreeRam(), DEC);
  
  //Initialise the module
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    
    flashLed(0, RED, 0);
  } 
  // Optional SSID scan
  // listSSIDResults();
}

void connectToWifi() { 
  
  wifiOrangeLight();

  Serial.print(F("\nAttempting to connect to ")); 
  Serial.println(WLAN_SSID);
  while(!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    flashLed(10, RED, 0);
    Serial.println(F("Trying again"));
  }

  Serial.println(F("Connected!"));

  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  wifiGreenLight();
  
  //Display the IP address DNS, Gateway, etc.
  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

}



void wifiError() { 

  while(1) {
    leds.setPixelColor(0, (millis()%400 < 200) ? 0x00000 : 0x110000); 
    leds.show();  
    delay(10); 
  };

}


void connectToSocketServer(char* webSite, int port) { 
  
  
  ip = 0;
  // Try looking up the website's IP address
  Serial.print(webSite); 
  Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(webSite, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }

  cc3000.printIPdotsRev(ip);



  // Optional: Do a ping test on the website
  //  uint16_t replies;
  //  Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");  
  //  replies = cc3000.ping(ip, 5);
  //  Serial.print(replies); Serial.println(F(" replies"));


  /* Try connecting to the website.
   Note: HTTP/1.1 protocol is used to keep the server from closing the connection before all data is read.
   */
  client = cc3000.connectTCP(ip, port);

  while(!client.connected()) { 
    Serial.println(F("Socket Connection failed"));    
    flashLed(10, RED, 1);  
    Serial.println(F("Trying again"));  

    client = cc3000.connectTCP(ip, PORT);

  }

  Serial.println(F("Socket Connection worked!"));    
  socketGreenLight();
  client.fastrprint("HELLO\0"); 
  
  
}
