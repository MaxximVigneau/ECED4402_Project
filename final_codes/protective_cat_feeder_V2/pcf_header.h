#ifndef _PCF_HEADER_H
#define _PCF_HEADER_H

  /* Librarys */ 
  #include "HX711.h"  //For Hx711 load cell amp and A/D converter functions
  #include<Servo.h>   //For servo motors Functions 
  
  /*Pin Defintions*/
  #define DT 3        //Data out is connected to D3 
  #define SCLK 2      //clock IN is conected to D2 
  #define SERVO1 10   //Servo1 is conected to D10 
  #define SERVO2 11   //Servo2 is connect to D11
  #define CV_LED 4    //Computer Vison Warning light is connected to D4 (RED LIGHT)
  #define BOWL_LED 5  //Bowl Timeout Warning light is connected to D5 (BLUE LIGHT) 

  /*Constant Variable Definitions*/
  const int calibration_factor = -9000;                 //Calibration factor for the scale
  const float cat_min = 1.0 ;                           //Smallest Weight the cat can trigger with one paw is 1.8 lbs, use 1 as a buffer
  const float cat_max = 13.0;                           //Largest weight the cat can trigger with full body on scale is 12.6 lbs, use 13 lbs as a buffer
  const unsigned long computer_vision_timeout = 1000;   //Will only wait 1000ms (1 second) for the computer vision progrm to return a value
  const unsigned long bowl_timeout = 120000;            //The bowl can only be open for a maxxium of 2 minutes
  const int bowl_open = 90;                             //angle the servos will be at when the bowl is open
  const int bowl_closed = 0;                            //angle the servos will be at when the bowl is closed

  /*Object Definitions*/
  HX711 scale;    //Declaring a object of type HX711
  Servo Joint1;   //Declaring a Servo object
  Servo Joint2;   //Declaring a Servo object

  /*Function Protoypes*/
  void computer_vision();
  void openning();
  void is_open();
  void closing();
  void check_failure();

#endif // _PCF_HEADER_H  //
