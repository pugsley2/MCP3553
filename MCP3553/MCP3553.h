#define MISO 12  // pin 12 : SPI data input
#define SCLOCK 13 // pin 13 : SPI clock pin
#define CHIPSEL 6 // pin 10 : Slave Chip Select 1
#define SNSR_PWR 7
#define I2C_ENBL A1 // A1
#include <SPI.h>

#pragma once
class MCP3553 {

  public:

    MCP3553();

    // initialize ADC in continous conversion
    void begin();

    // returns ADC value
    long read();

    unsigned long readUnsigned();

    // sleeps ADC
    void end();

  private:

    //used to store conversion
    union SPI_data {
      byte packet[4];
      unsigned long value;
    }data;

    // gets and process SPI Data from ADC 
    void conversion();


};

MCP3553::MCP3553() {
}

void MCP3553::begin() {
  pinMode(MISO, INPUT_PULLUP);
  pinMode(SCLOCK, OUTPUT);
  pinMode(CHIPSEL, OUTPUT);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV32);	// SPI clock rate < 5 MHz per MCP3550 spec
  SPI.setBitOrder(MSBFIRST);		// MSB or LSB first
  SPI.setDataMode(SPI_MODE0);		// rising/falling edge of clock
  // start continuous conversion
  digitalWrite(CHIPSEL, LOW);
}

long MCP3553::read() {
  conversion();
  return data.value;
}

unsigned long MCP3553::readUnsigned() {
  return (unsigned long) MCP3553::read();
}

void MCP3553::conversion() {
  
  // wait for ADC to finish a conversion
  if (!digitalRead(MISO)) {
    // transfer data
    data.packet[3] = SPI.transfer(0x55);
    data.packet[2] = SPI.transfer(0xAA);
    data.packet[1] = SPI.transfer(0x55);
    data.packet[0] = SPI.transfer(0xAA);
    //shift out unused bits
    data.value >>= 7;
    // get sign and mask accordingly
    if(data.value & (1L << 21)) {
      data.value |= 0xFFC00000;
    } else {
      data.value &= ~0xFFC00000;
    }
  }
}

void MCP3553::end() {
  digitalWrite(CHIPSEL, HIGH);
}