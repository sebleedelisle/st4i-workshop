#ifndef __WIFLY_SERVER_H__
#define __WIFLY_SERVER_H__

#include "Server.h"

class WiFlyClient;

class WiFlyServer : 
public Server {

public:
  WiFlyServer(uint16_t);
  WiFlyClient available();
  virtual void begin();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  using Print::write;

private:
  uint16_t _port;
  WiFlyClient activeClient;
};

#endif
