/* Contains functions related to the Atlas Scientific EC EZO. */

#define ec_address 100

float get_conductivity(){
  /* Get conductivity from the EC EZO.
   *  There is a built-in delay of 600ms 
   *  to allow the conductivity circuit to perform
   *  its conversion.
   *  return -- a float value representing electrical conductivity in uS/cm.
   */
  char ec_data[32]; //Responses from the EC EZO are usually 32 bytes.
  byte i = 0;
  byte incoming;
  char *ec;
  float ec_f;
  Wire.beginTransmission(ec_address);
  Wire.write(114);  //ASCII for R.
  Wire.endTransmission();
  delay(600);
  Wire.requestFrom(ec_address,48,1);
  Wire.read();
  while (Wire.available()){ //While wire is instantiated...
    incoming = Wire.read();  //Read the response from the EC EZO.
    ec_data[i] = incoming;
    i += 1; 
    if (incoming == 0){ //If there are no more bytes incoming...
      i=0;
      break;
    }
  }
  if (isDigit(ec_data[0])){  //If the data is a digit, convert from string to float.
    ec = strtok(ec_data,",");
    ec_f = atof(ec);
    return ec_f;
  }  
}

void power_ec_led(String state){
  /* Turn the EC EZO LED on or off.
   * Circuit must be in I2C mode.
   * state -- must be either "ON" or "OFF"
   * Nothing is returned from this function.
   */
  Wire.beginTransmission(ec_address);
  Wire.write(76); 
  Wire.write(44); 
  if (state == "ON"){
    Wire.write(49); 
  } 
  else if (state == "OFF"){
    Wire.write(48);  
  }
  Wire.endTransmission();
}
