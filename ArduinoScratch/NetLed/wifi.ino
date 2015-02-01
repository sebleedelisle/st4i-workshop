



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

  bool connected = false; 
  
  while(!connected) {
  
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
    int attempts = 0; 
    while ((!cc3000.checkDHCP()) && (attempts<5000)) {
      delay(100); // ToDo: Insert a DHCP timeout!
      Serial.print("."); 
      attempts++; 
    }  
    
    if(attempts<50)  { 
      connected = true;
      Serial.println("DHCP WORKED"); 
    } else { 
      connected = false; 
      cc3000.disconnect(); 
      Serial.println("DHCP FAILED"); 
    }
    
  }
  
  wifiGreenLight();
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  //Display the IP address DNS, Gateway, etc.
  
  //while (! displayConnectionDetails()) {
  //  delay(1000);
 //
}



void wifiError() { 

  while(1) {
    leds.setPixelColor(0, (millis()%400 < 200) ? 0x00000 : 0x110000); 
    leds.show();  
    delay(10); 
  };

}


bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}


