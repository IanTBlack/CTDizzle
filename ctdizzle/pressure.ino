/* Contains functions related to the Blue Robotics MS5837-30Bar pressure sensor. */

#include <MS5837.h>
MS5837 psensor;

float get_absolute_pressure(){
  /*Get the pressure as read by the sensor.
   * return -- a float value representing absolute pressure in millibars.
   */
  psensor.read();
  delay(50);
  return psensor.pressure();
}

float get_relative_pressure(float atmospheric){
  /* Compute relative pressure after taking a pressure sample.
   *  atmospheric -- a float value that is either taken during
   *  setup (assuming the sensor is above water) or a user selected value.
   *  Standard atmospheric pressure is 1013.25 mbar.
   *  return -- a float value representing relative pressure.
   */
  float absolute = get_absolute_pressure();
  float dbar = (absolute - atmospheric)/100;
  if (dbar < 0){dbar = 0;}
  return dbar;
}

void initialize_pressure(){
  /*Start up the pressure sensor and set the model.
   * Nothing is returned from this function.
   */
  psensor.init();
  psensor.setModel(MS5837::MS5837_30BA);
  delay(100);
  Serial.println("MS5837 initialized!");
}
