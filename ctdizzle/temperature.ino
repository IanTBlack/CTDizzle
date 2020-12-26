/* Contains functions related to the Blue Robotics TSYS01 temperature sensor. */

#include <TSYS01.h>
TSYS01 tsensor;

float get_temperature(){
  /*Get the temperature from the temperature sensor.
   * return -- a float value representing temperature in Celsius.
   */
  tsensor.read();
  delay(50);
  return tsensor.temperature();
}

void initialize_temperature(){
  /* Initialize the temperature sensor.
   * Nothing is returned from this function.
   */
  tsensor.init();
  delay(100);
  Serial.println("TSYS01 initialized!");
}
