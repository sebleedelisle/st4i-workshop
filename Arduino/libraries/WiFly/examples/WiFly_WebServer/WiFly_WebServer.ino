/*
 * Web Server
 *
 * (Based on Ethernet's WebServer Example)
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include <SPI.h>
#include <SC16IS750.h>
#include <WiFly.h>

WiFlyServer server(80);

void setup() {
  SC16IS750.begin();
  
  WiFly.setUart(&SC16IS750);
  
  Serial.begin(9600);  
  
  WiFly.begin();
  server.begin();
  
  Serial.println("Setting up WiFly...");
  
  WiFly.sendCommand(F("set wlan auth 0"));
  WiFly.sendCommand(F("set wlan channel 0"));
  WiFly.sendCommand(F("set ip dhcp 1"));

  if (!WiFly.sendCommand(F("join automata_arduino"), "Associated!", 20000, false)) {
    Serial.println(F("Association failed."));
    while (1) {
      // Hang on failure.
    }
  }
  
  if (!WiFly.waitForResponse("DHCP in", 10000)) {
    Serial.println(F("DHCP failed."));
    while (1) {
      // Hang on failure.
    }
  }

  Serial.print("IP: ");
  Serial.println(WiFly.localIP());
  
  server.begin();
}

void loop() {
  WiFlyClient client = server.available();
  
  delay(100);
  
  if (client.connected()) {
    
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == '\n' && current_line_is_blank) {
          
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          // output the value of each analog input pin
          for (int i = 0; i < 6; i++) {
            client.print("analog input ");
            client.print(i);
            client.print(" is ");
            client.print(analogRead(i));
            client.println("<br />");
          }
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(100);
    client.stop();
  }
}
