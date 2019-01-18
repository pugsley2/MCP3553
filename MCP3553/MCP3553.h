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

