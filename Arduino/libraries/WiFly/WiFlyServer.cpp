#include "WiFly.h"
#include "WiFlyClient.h"
#include "WiFlyServer.h"

#define TOKEN_MATCH_OPEN "*OPEN*"

WiFlyServer::WiFlyServer(uint16_t port) {
  _port = port;
}

void WiFlyServer::begin() {
  String cmd = "set ip localport ";
  cmd += _port;

  int len = cmd.length() + 1;
  char cmdbuf[len];
  cmd.toCharArray(cmdbuf, len);

  WiFly.sendCommand(cmdbuf);
}

WiFlyClient WiFlyServer::available() {

  if (!WiFly.serverConnected) {

    activeClient.reset();

    if (WiFly.uart->available() >= strlen(TOKEN_MATCH_OPEN)) {

      if (WiFly.waitForResponse(TOKEN_MATCH_OPEN, 2000)) {

        WiFly.serverConnected = true;
        activeClient.connect();

      } else {
        WiFly.uart->flush();
      }
    }
  }

  return activeClient;
}

size_t WiFlyServer::write(uint8_t b) {
  return write(&b, 1);
}

size_t WiFlyServer::write(const uint8_t *buffer, size_t size) {
  return activeClient.write(buffer, size);
}