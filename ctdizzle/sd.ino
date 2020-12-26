/* Contains functions related to the Adafruit Adalogger FeatherWing SD module. */

#include <SD.h> 
#define sd_cs 10
File csv;
File cfg;

void initialize_logger(){ 
  /* Start up the SD card, check for a config.txt file,
   *  and make a new datafile.
   */
  pinMode(sd_cs,OUTPUT);
  if (SD.begin(sd_cs)){
    delay(100);
    make_directory();
    delay(100);
    make_file();
    SdFile::dateTimeCallback(set_file_creation_datetime);
    read_config();
  }
  else{
    Serial.println("SD FAIL");
    while(1){
      error_cycle_led();
    }
  }
}

void log_data(String dt, float ec, float t, float p, float v){
  /* Log data to the csv file. 
   *  To change the output, you will need to modify this function in combination with 
   *  the make_file function (to change the header).
   *  Nothing is returned from this function.
   */
  char fmt[128];
  sprintf(fmt,"%f,%f,%f,%f",ec,t,p,v); //Combine comma separated float data.
  String data_string = String(dt) + "," + String(fmt);
  if (csv){
    csv.println(data_string);
    csv.flush();
    Serial.println(data_string);
  }
  else{
    while(1){
      error_cycle_led();
    }
  }
}

void make_directory(){
  /* Make a directory that is named by the current year.
   *  Data is stored in this directory. 
   *  Nothing is returned from this function.
   */
    DateTime now = rtc.now();
    String yyyy = String(now.year());
    SD.mkdir(yyyy);  
    Serial.println("Moving to root/" + yyyy + ".");
}

void make_file(){
    /* Create a file with a name in the format of
     *  mmddHHMM.csv.
     *  Nothing is returned from this function.
     */
    DateTime now = rtc.now();
    String yyyy = String(now.year());
    char name_buffer[32];
    sprintf(name_buffer,"%02u%02u%02u%02u",now.month(),now.day(),now.hour(),now.minute());
    String filename = String(name_buffer) + ".csv";
    String filepath = yyyy + "/" + filename; 
    Serial.println("Data will be located at " + filepath + ".");
    csv = SD.open(filepath,FILE_WRITE);
    delay(100);
    if(csv){  //If the data file was created, add a header line.
      String header = "datetime,conductivity,temperature,pressure,voltage"; 
      csv.println(header);  
      csv.flush();    
      Serial.println("Log file initialized!");
    }
}

void read_config(){
  /* Read in the contents of config.txt
   *  located on the SD card.
   *  Currently, only the sampling rate can be set 
   *  using config.txt.
   *  If unable to read the file or the contents are
   *  garbled, the default sampling rate is set to 
   *  1 sample per second.
   *  Nothing is returned from this function.
   */
   int idx =0;
   char cfg_buffer[32];
   cfg = SD.open("config.txt",FILE_READ);
   if (cfg){
    Serial.println("Reading configuration file.");
    while (cfg.available()){     
      cfg_buffer[idx] = cfg.read();
      idx++;
    }
    cfg.close();
    interval = atoi(cfg_buffer);
    if (int(interval)){
      Serial.println("Setting sampling rate to one sample every " + String(int(interval)) + " second(s).");
      Serial.println("Configuration file read.");      
    }
    else{
      Serial.println("Unable to interpret interval setting, defaulting to 1 sample per second.");
      interval = 1;
    }
   }
   else{
    interval = 1;
    Serial.println("Unable to read config.txt, defaulting to 1 sample per second.");
   }
}

void set_file_creation_datetime(uint16_t* date, uint16_t* time){
  /*Stolen from KWM. Sets the file creation time for use with a file explorer.*/
  DateTime now = rtc.now();
  *date = FAT_DATE(now.year(), now.month(), now.day());
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}
