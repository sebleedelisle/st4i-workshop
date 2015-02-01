#include "WiFly.h"
#include "WiFlyClient.h"


WiFlyClient::WiFlyClient() : _WiFly (WiFly) {
  connections = 0;
  closeCmd = "*CLOS*";
  closeBufferIndex = closeBufferSize = 0;    
}

// handle connect from server class
int WiFlyClient::connect() {
  connections = 1;
  return connections;
}

int WiFlyClient::connect(const char* host, uint16_t port) {
  String cmd = String("open ");
  cmd += host;
  cmd += " ";
  cmd += port;

  int len = cmd.length() + 1;
  char cmdbuf[len];
  cmd.toCharArray(cmdbuf, len);

  connections = _WiFly.sendCommand(cmdbuf, "*OPEN*", 10000, false);

  return connections;
}

int WiFlyClient::connect(IPAddress ip, uint16_t port) {
  String cmd = String("open ");
  cmd += ip[0];
  cmd += ".";
  cmd += ip[1];
  cmd += ".";
  cmd += ip[2];
  cmd += ".";
  cmd += ip[3];      
  cmd += " ";
  cmd += port;

  int len = cmd.length() + 1;
  char cmdbuf[len];
  cmd.toCharArray(cmdbuf, len);

  connections = _WiFly.sendCommand(cmdbuf, "*OPEN*", 10000, false);

  return connections;
}

size_t WiFlyClient::write(uint8_t value) {
  return _WiFly.uart->write((byte)value);
}

size_t WiFlyClient::write(const uint8_t *buf, size_t size) {
  return _WiFly.uart->write(buf, size);
}

int WiFlyClient::available() {
  if (connections < 1) {
    return 0;
  }

  return _WiFly.uart->available();
}

int WiFlyClient::read() {
  if (connections < 1) {
    return -1;
  }

  return bufferedRead();
}

int WiFlyClient::read(uint8_t *buf, size_t size) {
  uint8_t value;
  uint8_t count = 0;

  if (connections < 1) {
    return -1;
  }  

  for (uint8_t i=0; i<size; ++i) {
    value = bufferedRead();

    if (value > -1) {
      buf[count] = value;
      ++count;

    } else {
      break;
    }
  }

  return count;
}

int WiFlyClient::bufferedRead() {
  int value;

  if (closeBufferSize > 0) {
    value = closeBuffer[closeBufferIndex];

    ++closeBufferIndex;
    if (closeBufferIndex == closeBufferSize) {
      closeBufferIndex = closeBufferSize = 0;
    }

  } else {
    value = _WiFly.uart->read();

    while (value != -1 && closeBufferSize < sizeof(closeCmd) && closeCmd[closeBufferSize] == (char)value) {
      closeBuffer[closeBufferSize] = value;

      while (!_WiFly.uart->available()) {
        delay(1);
      }

      value = _WiFly.uart->read();
      ++closeBufferSize;  
    }

    // found close, kill connection
    if (closeBufferSize == sizeof(closeCmd)) {

      connections = 0;
      closeBufferSize = 0;
      closeBufferIndex = 0;

      _WiFly.uart->flush();

      // catch any stragglers
      delay(250);

      // empty the buffer
      while (_WiFly.uart->available()) {
        _WiFly.uart->read();
      }

      _WiFly.serverConnected = false;

      value = -1;

    // otherwise read from the buffer
    } else {
      if (closeBufferSize > 0) {
        value = closeBuffer[0];
        closeBufferIndex = 1;

        if (closeBufferIndex == closeBufferSize) {
          closeBufferIndex = closeBufferSize = 0;
        }
      }
    }
  }

  return value;
}

int WiFlyClient::peek() {
  if (connections < 1) {
    return -1;
  }  
  return _WiFly.uart->peek();
}

void WiFlyClient::flush() {
  if (connections < 1) {
    return;
  }
  
  _WiFly.uart->flush();
}

void WiFlyClient::stop() {
  _WiFly.sendCommand(F("close"), _WiFly.firmwareVersion);

  connections = 0;

  while (_WiFly.uart->available()) {
    _WiFly.uart->read();
  }

  _WiFly.serverConnected = false;
}

void WiFlyClient::reset() {
  
  connections = 0;
}

uint8_t WiFlyClient::connected() {
  return connections;
}

WiFlyClient::operator bool() {
  return connections == 0;
}
