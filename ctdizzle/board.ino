/* Contains functions generally related to the Adafruit Feather M0 Basic Proto. */

#include <Wire.h>
#include <SPI.h> 
#define vbatt_pin A7 

void error_cycle_led(){
  /* Cycle the board LED to
   *  give the user a visual indicator if an error occurs.
   *  Nothing is returned from this function.
   */
  power_board_led("ON");
  delay(1000);
  power_board_led("OFF");
  delay(1000);
}

float get_voltage(){
  /* Read the battery voltage on the vbatt pin.
   *  If under USB power, but not battery, this function will still produce a value.
   *  return -- a floating point value representing the voltage at the vbatt pin.
   */
  float vbatt = analogRead(vbatt_pin);
  vbatt *= 2;
  vbatt *= 3.3;
  vbatt /= 1024;
  return vbatt;
}

void initialize_comms(int bps, int i2c_clock){
  //Jump start serial and I2C protocols.
  Serial.begin(bps);
  Wire.begin();  
  delay(1000);
  Wire.setClock(i2c_clock);
}
  
void power_board_led(String state){
  /*Enable or disable the board led.
   * Nothing is returned from this function.
   */
  pinMode(LED_BUILTIN, OUTPUT);
  if (state == "ON"){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else if (state == "OFF"){
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
}

void power_leds(String state){
  /*Enable or disable the board and EC EZO leds.
   * Nothing is returned from this function.
   */
  if (state == "ON"){
    power_board_led("ON");
    power_ec_led("ON");
  }
  else if (state == "OFF"){
    power_board_led("OFF");
    power_ec_led("OFF");
  }  
}

void wait_for_serial(int seconds){
  /* Wait for a serial connection for X number of seconds. 
   *  If no connection is made, the CTD runs through setup, and if no errors are found, it begins sampling.
   * seconds -- number of seconds to wait for a serial connection before moving on.
   */
  while(!Serial){
    error_cycle_led();
    if (millis() > seconds*1000){
      break;
    }
  }
  Serial.println("------------------------------------------------------------");
}
