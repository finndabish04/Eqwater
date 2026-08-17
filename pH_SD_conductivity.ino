
// SD initialization code 
// DO NOT CHANGE THIS PART
// YES FINN THAT MEANS YOU
/* ====================================== +++++ ====================================== */
#define USE_SD_H 0
#if USE_SD_H
#include <SD.h>
#else  // USE_SD_H
#include "SdFat.h"
SdFat SD;
#endif  // USE_SD_H

File myFile;
const int SD_CS_PIN = 18;

//CONSTANTS AND VARIABLES FOR PH
/* ====================================== +++++ ====================================== */
int phPin = 4;
float avg[100];
float juice_voltage[100];
float neutralPoint_pH = 7.0;
float otherPoint_pH = 3.0;
float acidVolt = 730.07;
float neutralVolt = 1050.0; //approximation
/* ====================================== +++++ ====================================== */


void setup() {
  // put your setup code here, to run once:
  pinMode(phPin, INPUT);

  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 17, 16); // serial begin for other conductivity thing
  delay(2000);
  Serial.println("Serial Ready!");

//MORE SD CODE
//DO NOT TOUCH
//I REPEAT
//DO NOT TOUCH
/* ====================================== +++++ ====================================== */
#if USE_SD_H
  Serial.println(F("Using SD.h. Set USE_SD_H zero to use SdFat.h."));
#else   // USE_SD_H
  Serial.println(F("Using SdFat.h. Set USE_SD_H nonzero to use SD.h."));
#endif  // USE_SD_H
  Serial.println(F("\nType any character to begin."));
  while (!Serial.available()) {
    yield();
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing with pH.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  /* ====================================== +++++ ====================================== */
}


void loop() {

  Serial.println("Begin Data Collection: ");

  myFile = SD.open("test.txt", FILE_WRITE);

//PH CODE 
//THIS WILL BE CHANGED LATER
/* ====================================== +++++ ====================================== */
  int count = 0;
  int phRead;
  float total = 0;
  int samples = 25;

  for(count; count < samples ; count++){
    phRead = analogRead(phPin) * 3300.0 / 4096.0;
    avg[count] = phRead;
    total += phRead;

    Serial.print("Count = ");
    Serial.println(count);
    Serial.print("pH Voltage = ");
    Serial.println(phRead);

    myFile.print("Voltage at the pH pin is: ");
    myFile.println(phRead);

    delay(150);
  }
  
  Serial.println("===============+++++===============");
  Serial.print("Average voltage = ");
  float avgVoltage = (total/samples);
  Serial.println(avgVoltage);
  Serial.println(readPH(avgVoltage, 1040.0, 750.0));

  myFile.print("pH is: ");
  myFile.println(readPH(avgVoltage, 1040.0, 750.0));
  Serial.println("===============+++++===============");

  delay(5000);

  //CONDUCTIVITY CODE!!
  /* ====================================== +++++ ====================================== */
  Serial1.print("R\r"); 
  delay(1000); 

  String ec_val = "";
  bool validEC = false;

  unsigned long start = millis();
  while (millis() - start < 2000) {  // wait up to 2 seconds
    if (Serial1.available()) {
      ec_val = Serial1.readStringUntil('\r');
      ec_val.trim();

      if (ec_val.length() > 0 && ec_val[0] != '*') {
        Serial.print("EC: ");
        Serial.println(ec_val);
        validEC = true;
        break;
      }
    }
  }

  delay(5000);

  if(validEC){ // if statement to make sure EC is good data
      myFile.println(ec_val);
    } else {
      myFile.println("NaN"); // NaN is recognized as missing data in excel, python, and matlab
    }
    
  myFile.close();
}
//END OF CODE
//JUST FUNCTION DEF UNDER THIS
/* ====================================== +++++ ====================================== */

float readPH(int analogVoltage, float neutralVoltage, float otherVoltage){
  //Convert calibration voltages to pH, assuming 1500 mV corresponds to a pH of 7.0
  float neutralPH = (neutralVoltage - 1500.0) / 3.0;
  float otherPH = (otherVoltage - 1500.0) / 3.0;

  //Use the known pH of the calibration points to calculate the slope between them
  float slope = (neutralPoint_pH - otherPoint_pH) / (neutralPH - otherPH);
  float intercept = neutralPoint_pH - (slope * neutralPH);

  return slope * ((analogVoltage - 1500.0) / 3.0) + intercept;
} 
/* ====================================== +++++ ====================================== */

