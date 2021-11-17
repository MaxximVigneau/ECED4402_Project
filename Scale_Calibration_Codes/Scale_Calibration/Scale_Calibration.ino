/* File: Scale_Calibration.c
   Purpose: This code can be used to find
            the calibration factor of the
            scale.
   Date: OCT 13 2021
   Notes: This code is modified from 
          Nathan Seidle, SparkFun 
          Electronics, November 19th 
          2014, SparkFun_HX711_
          Calibration.ino code
*/

#include "HX711.h" //Library for Hx711 load cell amplifier and A/D converter 

#define DT 3      //Data OUT is connect to D3
#define SCLK 2    //Clock IN is connect to D2

HX711 scale;      //Declaring a object of type HX711

float calibration_factor = -9000; //6150

void setup() {

  Serial.begin(9600);

  Serial.println("HX711 Calibration Sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place know weight on scale");
  Serial.println("Press a to increase calibration factor");
  Serial.println("Press z to decrease calibration factor");

  //Initializing Scale
  scale.begin(DT, SCLK);
  scale.set_scale();
  scale.tare();   //Resets the scale to 0
  
}

void loop() {
  // put your main code here, to run repeatedly:

  scale.set_scale(calibration_factor); //Adjust to the current calibration factor 

  Serial.print("Reading: ");
  Serial.print(scale.get_units(10),1);  //Print the current weight to the monitor 
  Serial.print(" lbs"); 

  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor); 
  Serial.println(); 

  if(Serial.available()){
    char temp = Serial.read(); //Read in input from the keyboard 

    if(temp == 'a'){
      calibration_factor += 10; //Increment the calibration factor by 10
    }else if(temp == 'z'){
      calibration_factor -= 10; //Decrement the calibration factor by 10
    }
  }

}
