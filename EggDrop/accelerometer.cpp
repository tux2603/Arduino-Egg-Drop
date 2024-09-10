#include "accelerometer.h"
#include "Arduino.h"
#include <Wire.h>

void Accelerometer::begin() {
  Wire.begin();

  write_register(Accelerometer::CTRL1_XL, 0xAE);
  write_register(Accelerometer::CTRL8_XL, 0x09);
}


bool Accelerometer::is_ready() {
  uint8_t status;
  uint8_t return_val;

  if (!(return_val = read_register(Accelerometer::STATUS, status))) {
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

  x = buffer[0] * 78.5 / 32768.0;
  y = buffer[1] * 78.5 / 32768.0;
  z = buffer[2] * 78.5 / 32768.0;

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

  uint8_t return_val;

  if (return_val = Wire.endTransmission(false)) {
    Serial.print("Error reading from accelerometer (");
    Serial.print(return_val);
    Serial.println(")");
    return return_val;
  }

  uint8_t rx_length;

  if ((rx_length = Wire.requestFrom(Accelerometer::ADDR, length)) != length) {
    Serial.print("Got unexpected number of bytes (");
    Serial.print(length);
    Serial.print(" expected, ");
    Serial.print(rx_length);
    Serial.println(" received)");
    return 6; 
  }

  for (size_t i = 0; i < length; ++i) {
    *data++ = Wire.read();
  }

  return 0;
}