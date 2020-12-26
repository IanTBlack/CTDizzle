/* Contains functions related to the Adafruit Adalogger FeatherWing PCF5823 RTC. */

#include "RTClib.h"
RTC_PCF8523 rtc;

String get_datetime(){
  /* Get the datetime in ISO8601 format.
   *  return -- the date and time in the format of
   *      yyyy-mm-ddTHH:MM:SS.
   */
  char dt_buffer[32];
  DateTime now = rtc.now();
  sprintf(dt_buffer,"%04u-%02u-%02uT%02u:%02u:%02u",now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second());
  delay(50);
  return String(dt_buffer);
}

int get_unixtime(){
  /* Gets the current time as seconds since
   *  1970-01-01 00:00:00
   *  return -- a float value representing seconds since the UNIX epoch.
   *  NOTE: Will probably cause errors after 2038-01-19.
   */
  DateTime now = rtc.now();
  int unix = now.unixtime();
  return unix;
}

void initialize_rtc(){
  /* Check the RTC status. 
   *  Resets the time if a serial connection is 
   *  available and the RTC lost power.
   *  Nothing is returned from this function.
   */
  if (rtc.begin()){
    delay(100);
    if (!rtc.initialized()){ //If the RTC lost power and there is a serial connection...
      if (Serial){
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));         
      }
      else{
        while(1){
          error_cycle_led();          
        }
      }
    }
    delay(1000);
    Serial.println("RTC initialized!");
  }
  else{
    Serial.println("RTC NOT initialized."); 
    while(1){
      error_cycle_led();      
    }
  }
}
