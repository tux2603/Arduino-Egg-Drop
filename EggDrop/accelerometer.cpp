#include "accelerometer.h"
#include "Arduino.h"
#include <Wire.h>

void Accelerometer::begin() {
  Wire.begin(); // TODO: should this be done here or outside of the class?

  // Set accelerometer to 6.66 kHz, +/- 8g, 100Hz AA
  write_register(Accelerometer::CTRL1_XL, 0xAE);
  write_register(Accelerometer::CTRL8_XL, 0x01);
}


bool Accelerometer::is_ready() {
  uint8_t status;

  if (!read_register(Accelerometer::STATUS, status)) {
    return (status & 0x01) ? true : false;
  }

  return false;
}


bool Accelerometer::read(float &x, float &y, float &z) {
  int16_t buffer[3];

  if (read_register(Accelerometer::OUT_XL, (uint8_t *)buffer, 6)) {
    x = NAN;
    y = NAN;
    z = NAN;

    return false;
  }

  x = buffer[0] * 4.0 / 32768.0;
  y = buffer[1] * 4.0 / 32768.0;
  z = buffer[2] * 4.0 / 32768.0;

  return true;
}


uint8_t Accelerometer::write_register(uint8_t addr, uint8_t data) {
  Wire.beginTransmission(Accelerometer::ADDR);
  Wire.write(addr);
  Wire.write(data);

  return Wire.endTransmission();
}


uint8_t Accelerometer::read_register(uint8_t addr, uint8_t &data) {
  return read_register(addr, &data, 1);
}


uint8_t Accelerometer::read_register(uint8_t addr, uint8_t *data, size_t length) {
  Wire.beginTransmission(Accelerometer::ADDR);
  Wire.write(addr);

  uint8_t return_val = 0x00;

  if (!(return_val = Wire.endTransmission(false))) {
    return return_val;
  }

  if (Wire.requestFrom(Accelerometer::ADDR, length) != length) {
    return 4;
  }

  for (size_t i = 0; i < length; ++i) {
    *data++ = Wire.read();
  }

  return 0;
}