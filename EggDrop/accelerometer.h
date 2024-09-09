#ifndef accelerometer_h
#define accelerometer_h

#include "Arduino.h"

class Accelerometer {
  public:
    Accelerometer() {}

    void begin();
    bool is_ready();
    bool read(float &x, float &y, float &z);

  private:
    const static uint8_t ADDR = 0x64;
    const static uint8_t CTRL1_XL = 0x10;
    const static uint8_t CTRL2_G = 0x11;
    const static uint8_t CTRL7_G = 0x16;
    const static uint8_t CTRL8_XL = 0x17;
    const static uint8_t STATUS = 0x1E;
    const static uint8_t OUT_XL = 0x28;
    uint8_t write_register(uint8_t addr, uint8_t data);
    uint8_t read_register(uint8_t addr, uint8_t &data);
    uint8_t read_register(uint8_t addr, uint8_t *data, size_t length);

};

#endif