#include <FlashStorage_SAMD.h>
#include <Wire.h>
#include "accelerometer.h"

Accelerometer accel;

float max_accel = 0.0;

void save_accel(float accel);
float read_saved_accel();

void setup() {
  // Start talking to the computer, if it's plugged in
  Serial.begin(115200);

  while(!Serial);

  // Get the accelerometer ready for reading
  accel.begin();

  // Read the previous maximum acceleration experienced
  max_accel = read_saved_accel();

  Serial.print("Maximum acceleration recorded is ");
  Serial.print(max_accel);
  Serial.println(" m/s^2");
}

void loop() {
  // The accelerometer reads acceleration along each x, y, and z axis
  // Make variables to store each of those individual accelerations
  float x_accel, y_accel, z_accel;

  // With these three individual accelerations, we will calculate the overall
  //  acceleration of the arduino using the pythagorean theorem
  // Make a variable to store this overall acceleration
  float overall_accel;

  // If there is a value to be read from the accelerometer, go ahead and ready it
  if (accel.is_ready()) {
    // Read the individual accelerations
    accel.read(x_accel, y_accel, z_accel);

    // Calculate the overall acceleration
    overall_accel = sqrt(x_accel * x_accel + y_accel * y_accel + z_accel * z_accel);

    // If this new overall accelration is greater than the previous maximum acceleration
    //  we've seen, store it as the new maximum acceleration
    if (overall_accel > max_accel) {
      max_accel = overall_accel;
      save_accel(max_accel);
    }
  }

  // If an 'r' was typed on the serial connection, reset the max accel
  if (Serial.available()) {
    char input = Serial.read();

    if(input == 'r') {
      max_accel = 0;
      save_accel(max_accel);

      Serial.println("Reset acceleration reading!");
    }
  }
}



// ########################################################
// ##### Don't worry about anything below this point! #####
// #####      It's complicated bookkeeping stuff      #####
// ########################################################

void save_accel(float accel) {
  uint8_t *buffer = (uint8_t *)&accel;

  for (size_t i = 0; i < sizeof(float); ++i) {
    EEPROM.write(i, buffer[i]);
  }

  EEPROM.commit();
}


float read_saved_accel() {
  float accel = 0.0;
  uint8_t *buffer = (uint8_t *)&accel;

  for (size_t i = 0; i < sizeof(float); ++i) {
    buffer[i] = EEPROM.read(i);
  }

  if (isnan(accel)) {
    accel = 0.0;
  }

  return accel;
}