/*
 Description: A sketch for operating the CTDizzle at a defined sampling rate.
 - To set the sampling rate, you can change the value in the config.txt file on the SD card. 
   Alternatively, you can set the interval value in the loop function to X number of seconds. 
 - To modify the header information, you will need to modify make_file under sd.ino.
 - To modify the data output, you will need to modify log_data under sd.ino.
 - In the event of a failure (e.g. no sd card, can't communicate with clock, etc),
   the board led will cycle on/off once per second.
 - To modify the sketch for a different board, be sure to check the static definitions
   at the top of each .ino file. 
 - Pressure is sampled at the start, under the assumption that the sensor is above water. 
   This assists in computation of relative ocean pressure later.
   Alternatively, standard atmospheric pressure is 1013.25 mbar.
 
 * Ian Black, 2020-12-26
 */

float atmospheric_pressure;  //Declare globally so that we can pass it from setup to loop.
int interval;  //Declare globally so we can read it from the config file and pass it to loop.

void setup() {
  initialize_comms(115200,400000);  //Set the serial and I2C clock speeds .
  wait_for_serial(30); //Wait for 30 seconds for a serial connection. If no connection, continue sampling.
  power_leds("ON"); //Turn on all leds for a visual indicator.
  initialize_rtc(); 
  initialize_logger();  
  initialize_temperature();
  initialize_pressure(); 
  atmospheric_pressure = get_absolute_pressure(); //Assuming you turned on the sensor above water, get the atmospheric pressure.
  power_leds("OFF"); //Turn off all leds to save power.
}

void loop() {
  int start_millis = millis(); //Get the number of milliseconds since the board started up.
  float v = get_voltage();   
  float ec = get_conductivity();   
  float p = get_relative_pressure(atmospheric_pressure); 
  float t = get_temperature(); 
  String dt = get_datetime();
  log_data(dt,ec,t,p,v); //Log data to a file on the SD card.
  int end_millis = millis(); //Subtracting start_millis from this value will give how fast it takes to run the loop.
  delay((interval*1000)-(end_millis-start_millis)); //Subtract (end-start) from interval to get the number of milliseconds to wait to get a more accurate sampling rate.
}
