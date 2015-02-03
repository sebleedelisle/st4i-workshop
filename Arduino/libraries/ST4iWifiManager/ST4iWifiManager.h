
#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Adafruit_CC3000.h>
#include <Arduino.h>
#include <SPI.h>

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

const int connectTimeout = 15L * 1000L;

class ST4iWifiManager { 

	public :

	bool connected; 
	Adafruit_CC3000 cc3000; 
	Adafruit_CC3000_Client client;

	int ledPin; 

	uint32_t ip,t, ledBrightness;
		char
		  country[20],
		  region[20],
		  city[20],
		  name[13],  // Temp space for name:value parsing
		  value[64]; // Temp space for name:value parsing
		float
		  longitude, latitude;
		
	ST4iWifiManager() : cc3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
	SPI_CLOCK_DIVIDER)  { 
		ip = 0; 
		t = 0;

	};
			
	bool init(int ledpin = -1, int ledbrightness = 255) { 
		ledBrightness = ledbrightness; 
		ledPin = ledpin; 
		if(ledPin>-1) {
			
			pinMode(ledPin, OUTPUT); 
			for(int j = 0; j<4; j++) { 
				for(int i = 255; i>0; i--) { 
					analogWrite(ledPin, i); 
					delay(1); 
				}
			}
		}
		Serial.println(F("CC3000 Initializing..."));
		if (!cc3000.begin()) {
			Serial.println(F("Couldn't begin()! Check your wiring?"));
			return false; 
		} 
		return true;
	}
	
	bool monitor() { 
		
		if(cc3000.getStatus() == STATUS_CONNECTED) { 
			
			if(ledPin>-1) analogWrite(ledPin,ledBrightness); 
		} else { 
			if(ledPin>-1) digitalWrite(ledPin,(millis()%400)<200); 
		}
		
	}
	
	bool connect(const char* ssid, const char* pass, uint8_t security) { 
		
		Serial.print(F("\nAttempting to connect to ")); 
		Serial.println(ssid);
		 
		while(!cc3000.connectToAP(ssid, pass, security)) {
			Serial.println(F("Failed!"));
			//flashLed(10, RED, 0);
			delay(1000);
			Serial.println(F("Trying again"));
			
		}
				
		Serial.println(F("Connected!"));
		if(ledPin>-1) analogWrite(ledPin,ledBrightness); 
		
		/* Wait for DHCP to complete */
		Serial.println(F("Request DHCP"));
		while (!cc3000.checkDHCP()) {
			delay(100); // ToDo: Insert a DHCP timeout!
		}  
		
		//wifiGreenLight();
		
		//Display the IP address DNS, Gateway, etc.
		
		while (! displayConnectionDetails()) {
			delay(1000);
		}
		
		
		return true; 
	}
	
	
	// perhaps do an asynchronous version too? 
	String getWebPage( char* server, const char* path){
			
		// Look up server's IP address
		Serial.print("Opening "); 
		Serial.print(server); 
		Serial.println(path); 
		Serial.print(F("\r\nGetting server IP address..."));
		t = millis();
		ip = 0L; 
		while((0L == ip) && ((millis() - t) < connectTimeout)) {
			if(cc3000.getHostByName(server, &ip)) break;
			delay(1000);
		}
		if(ip == 0L) {
			Serial.println(F("failed"));
			return "";
		}
		cc3000.printIPdotsRev(ip);
		Serial.println();
	
		client = cc3000.connectTCP(ip, 80);
		if(client.connected()) {
			Serial.print(F("connected.\r\nRequesting data..."));
			cc3000.printIPdotsRev(ip);
			Serial.println("");
			
			client.fastrprint(F("GET "));
			client.fastrprint(path);
			client.fastrprint(F(" HTTP/1.1\r\n"));
			client.fastrprint(F("Host: ")); 
			client.fastrprint(server); 
			client.fastrprint(F("\r\n"));
			client.fastrprint(F("\r\n"));
		  
		} else {
			Serial.println(F("failed"));
			return "";
		}
		
		String ret = ""; 
		int c = 0; 
		
		unsigned long lastRead = millis(); 
		
		// todo check returned headers
		
		while (client.connected() && (millis() - lastRead < 3000)) {
	    	while (client.available()) {
		      	char c = client.read();
				ret+=(char)c; 
				Serial.print(c);
				lastRead = millis();
				if(ret.indexOf("\r\n\r\n")>-1) ret = ""; // strip out headers
		    }
		}
		client.close();

		return ret; 
	}
	
	char timedRead(void) {
		unsigned long start = millis();
		// hang around for 5 seconds while we wait for data
		while((!client.available()) && ((millis() - start) < 5000L));
		
		int c = client.read();  // -1 on timeout
	//	Serial.print(c); 
		return c; 
	}
	
	
	
	bool displayConnectionDetails(void) {
	  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

	  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)) {
	    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
	    return false;
	  } else {
	    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
	    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
	    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
	    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
	    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
	    Serial.println();
	    return true;
	  }
	}
};

#endif