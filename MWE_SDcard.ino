/*
* SD card shit for water project 
*/

#include "Arduino.h"
#include "SD.h"
#include "SPI.h"

const int chipSelect = 18;
File datafile;

void setup() {
  // Begin serial monitor
  Serial.begin(115200); 
  delay(3000);
  Serial.println("Serial loaded");
  //SPI.setFrequency(10);

  //Initialize SD card (and print if there is an error)
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("it didnt work :(");
    while (true);
  }
  Serial.println("initialization done.");

  datafile = SD.open("testSDcard.txt", FILE_WRITE);
}


void loop() {

  //print a string to the file 5 times
  String testString = "Please work im begging";
  static int numLoops = 0;
  if(datafile){
    datafile.println(testString);
    Serial.println("I wrote to the file o7");
  }
  numLoops++;

  if(numLoops > 5){
    datafile.close();
    Serial.print("all done!");
  }

  delay(2000);
}

/*
Notes:
- if you open and write to the file multiple times, it completely overwrites the entire file
- solution: open file once (in setup) and close only once at the end of loop (when it sleeps)
- "but finn, it will open in setup everytime it boots up!" 
- solution: check if file exists, if it does, simply append!
*/
