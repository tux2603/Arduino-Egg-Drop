#include <Wire.h>
#include "accelerometer.h"

Accelerometer accel;

float max_accel = 0.0;

void setup() {
  // Start talking to the computer, if it's plugged in
  Serial.begin(115200);

  // Get the accelerometer ready for reading
  accel.begin();
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
    }

    // Print out the current and maximum accelerations
    Serial.print(overall_accel);
    Serial.print(" ");
    Serial.println(max_accel);
  }
}
