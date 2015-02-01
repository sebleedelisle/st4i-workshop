
#include "SC16IS750.h"

// See section 8.10 of the datasheet for definitions
// of bits in the Enhanced Features Register (EFR)
#define EFR_ENABLE_CTS 1 << 7
#define EFR_ENABLE_RTS 1 << 6
#define EFR_ENABLE_ENHANCED_FUNCTIONS 1 << 4


// See section 8.4 of the datasheet for definitions
// of bits in the Line Control Register (LCR)
#define LCR_ENABLE_DIVISOR_LATCH 1 << 7


// The original crystal frequency used on the board (~12MHz) didn't
// give a good range of baud rates so around July 2010 the crystal
// was replaced with a better frequency (~14MHz).
#define USE_14_MHZ_CRYSTAL true // true (14MHz) , false (12 MHz)
#define XTAL_FREQUENCY 14745600UL // On-board crystal (New mid-2010 Version)


// See datasheet section 7.8 for configuring the
// "Programmable baud rate generator"
#define PRESCALER 1 // Default prescaler after reset
#define BAUD_RATE_DIVISOR(baud) ((XTAL_FREQUENCY/PRESCALER)/(baud*16UL))

// TODO: Handle configuration better
// SC16IS750 register values
struct SPI_UART_cfg {
  char DataFormat;
  char Flow;
};

struct SPI_UART_cfg SPI_Uart_config = {
  0x03,
  // We need to enable flow control or we overflow buffers and
  // lose data when used with the WiFly. Note that flow control 
  // needs to be enabled on the WiFly for this to work but it's
  // possible to do that with flow control enabled here but not there.
  // TODO: Make this able to be configured externally?
  EFR_ENABLE_CTS | EFR_ENABLE_RTS | EFR_ENABLE_ENHANCED_FUNCTIONS
};


void SC16IS750Device::begin(unsigned long baudrate /* default value */) {
  /*
        
   */
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  initUart(baudrate);
}


void SC16IS750Device::initUart(unsigned long baudrate) {
  /*
    
    Initialise the UART.
    
    If initialisation fails this method does not return.
        
   */
  // Initialise and test SC16IS750
  configureUart(baudrate);
  
  if(!uartConnected()){ 
    while(1) {
      // Lock up if we fail to initialise SPI UART bridge.
    }; 
  }
  
  // The SPI UART bridge is now successfully initialised.
}


void SC16IS750Device::setBaudRate(unsigned long baudrate) {
  /*
   */
  unsigned long divisor = BAUD_RATE_DIVISOR(baudrate);

  writeRegister(LCR, LCR_ENABLE_DIVISOR_LATCH); // "Program baudrate"
  writeRegister(DLL, lowByte(divisor));
  writeRegister(DLM, highByte(divisor)); 
}


void SC16IS750Device::configureUart(unsigned long baudrate) {
  /*
  
     Configure the settings of the UART.
  
   */
  // TODO: Improve with use of constants and calculations.
  setBaudRate(baudrate);

  writeRegister(LCR, 0xBF); // access EFR register
  writeRegister(EFR, SPI_Uart_config.Flow); // enable enhanced registers
  writeRegister(LCR, SPI_Uart_config.DataFormat); // 8 data bit, 1 stop bit, no parity
  writeRegister(FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
  writeRegister(FCR, 0x01); // enable FIFO mode   
}


boolean SC16IS750Device::uartConnected() {
  /*
  
     Check that UART is connected and operational.
  
   */
  // Perform read/write test to check if UART is working
  const char TEST_CHARACTER = 'H';
  
  writeRegister(SPR, TEST_CHARACTER);

  return (readRegister(SPR) == TEST_CHARACTER);
}


void SC16IS750Device::writeRegister(byte registerAddress, byte data) {
  /*

    Write <data> byte to the SC16IS750 register <registerAddress>.

   */
  digitalWrite(SS, LOW);
  SPI.transfer(registerAddress);
  SPI.transfer(data);
  digitalWrite(SS, HIGH);
}


byte SC16IS750Device::readRegister(byte registerAddress) {
  /*
  
    Read byte from SC16IS750 register at <registerAddress>.
  
   */
  // Used in SPI read operations to flush slave's shift register
  const byte SPI_DUMMY_BYTE = 0xFF; 
  
  char result;

  digitalWrite(SS, LOW);
  SPI.transfer(SPI_READ_MODE_FLAG | registerAddress);
  result = SPI.transfer(SPI_DUMMY_BYTE);
  digitalWrite(SS, HIGH);

  return result;  
}


int SC16IS750Device::available() {
  /*
  
    Get the number of bytes (characters) available for reading.

    This is data that's already arrived and stored in the receive
    buffer (which holds 64 bytes).
  
   */
  // This alternative just checks if there's data but doesn't
  // return how many characters are in the buffer:
  //    readRegister(LSR) & 0x01
  return readRegister(RXLVL);
}


int SC16IS750Device::read() {
  /*
  
    Read byte from UART.
   
    Returns byte read or or -1 if no data available.
    
    Acts in the same manner as 'Serial.read()'.
  
   */
  if (!available()) {
    return -1;
  }

  return readRegister(RHR);
}


size_t SC16IS750Device::write(byte value) {
  /*
  
    Write byte to UART.
 
   */
  while (readRegister(TXLVL) == 0) {
    // Wait for space in TX buffer
  };
  writeRegister(THR, value); 
}


size_t SC16IS750Device::write(const char *str, size_t size) {
  /*
  
    Write string to UART.
 
   */
	while (size--)
		write(*str++);  
	while (readRegister(TXLVL) < 64) {
		// Wait for empty TX buffer (slow)
		// (But apparently still not slow enough to ensure delivery.)
	};
}

void SC16IS750Device::flush() {
  /*
  
    Flush characters from SC16IS750 receive buffer.
  
   */
  // Note: This may not be the most appropriate flush approach.
  //       It might be better to just flush the UART's buffer
  //       rather than the buffer of the connected device
  //       which is essentially what this does.
  while(available() > 0) {
    read();
  }
}


void SC16IS750Device::ioSetDirection(unsigned char bits) {
  /*
   */
  writeRegister(IODIR, bits);
}


void SC16IS750Device::ioSetState(unsigned char bits) {
  /*
   */
  writeRegister(IOSTATE, bits);
}

boolean SC16IS750Device::reboot() {
  ioSetDirection(0b00000010);
  ioSetState(0b00000000);
  delay(1);
  ioSetState(0b00000010);

  //TODO: Handle hard reboot scenario
//    return findInResponse("*READY*", 2000);
  return true;
}

SC16IS750Device SC16IS750;
