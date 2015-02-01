#ifndef __WIFLY_CLIENT_H__
#define __WIFLY_CLIENT_H__
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"
#include "WiFlyDevice.h"

class WiFlyClient : public Client {

  public:
    WiFlyClient();

    int connect();

    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char *host, uint16_t port);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buf, size_t size);
    virtual int peek();
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();

    void reset();

  //  friend class WiFlyServer;
    
    using Print::write;

  private:
    int bufferedRead();

    WiFlyDevice& _WiFly;
    uint8_t connections;

    const char* closeCmd;
    int closeBuffer[7];
    int closeBufferSize;
    int closeBufferIndex;  
};

#endif