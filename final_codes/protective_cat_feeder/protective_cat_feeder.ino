/* File: protective_cat_feeder
   Programmer: Maxxim Vigneau
   Purpose: To create a program which prevents dogs
            from stealing cat food, using computer
            vision and a scale.
   Notes: Ensure to go to tools and select ATmega328P
          (Old Bootloder)
          Please note this implementation does not
          include computer vision
   Date: NOV 16 2021
*/

#include "HX711.h" //Library for Hx711 load cell amplifire and A/D converter 
#include<Servo.h> //Library for servo motors 


#define DT 3 //Data OUT is connected to D3 
#define SCLK 2 //clock IN is conect to D2 
#define SERVO1 10 //Servo output pin (must be PWM pin)
#define SERVO2 11 //Servo output pin (must be PWM pin) 
#define CV_LED 4 //Computer Vison warning light pin (RED LIGHT)
#define BOWL_LED 5 //Bowl Timeout warning light pin (BLUE LIGHT) 

HX711 scale; //Declaring a object of type HX711
Servo Joint1; //Declaring a Servo object
Servo Joint2; //Declaring a Servo object

const int calibration_factor = -9000; //Calibration factor for the scale
const float cat_min = 1.0 ; //Smallest Weight the cat can trigger with one paw is 1.8 lbs, use 1 as a buffer
const float cat_max = 13.0; //Largest weight the cat can trigger with full body on scale is 12.6 lbs, use 13 lbs as a buffer
const unsigned long computer_vision_timeout = 1000; //Will only wait 1000ms (1 second) for the computer vision progrm to return a value
const unsigned long bowl_timeout = 120000; //The bowl can only be open for a maxxium of 2 minutes
const int bowl_open = 90; //angle the servos will be at when the bowl is open
const int bowl_closed = 0; //angle the servos will be at when the bowl is closed


float weight = 0; //weight read in from scale
float precent_cat = 0; //value returned from the computer vision program
int angle = 0; //current location of the servo motors (0 == closed, 90 == open)
int cv_timeout_count = 0; //Tracks the number of times the computer vision software has failed to return a value in a row
int bowl_timeout_count = 0; //Tracks the number of times the bowl time out has occured in a row

void computer_vision();
void openning();
void is_open();
void closing();
void check_failure();

void setup() {

  Serial.begin(9600); //Inalizing serial communication, and setting the baud rate to 9600 bps

  /* Initializing Scale */
  scale.begin(DT, SCLK);
  scale.set_scale();
  scale.tare();
  scale.set_scale(calibration_factor);  //Applying the calibration factor to the sacle

  /*Initalizing Servo Motors*/
  Joint1.attach(SERVO1);
  Joint2.attach(SERVO2);
  Joint2.write(angle);
  Joint1.write(angle);

  /*Initalizing Warning Lights*/
  pinMode(CV_LED, OUTPUT);
  pinMode(BOWL_LED, OUTPUT);
  digitalWrite(CV_LED, LOW);
  digitalWrite(BOWL_LED, LOW);
}

void loop() {

  weight = scale.get_units(15); //get the weight currently applied to the scale

  if (weight > cat_min) {
    computer_vision();

    if ((weight < cat_max) && (precent_cat > 0.75)) {
      openning();
    }

  }

  check_failure();
}

void computer_vision() {

  unsigned long start_time;
  unsigned long current_time;
  bool data = false; //has the computer vision program returned a value
  precent_cat = 0;

  Serial.println("Check Camera"); //tell Rasberry Pi to run image classifer

  start_time = millis(); //miliseconds passed since the program started when image classifer started
  current_time = millis();

  while (((current_time - start_time) < computer_vision_timeout) && data == false) {

    if (Serial.available() > 0) {
      data = true;
      cv_timeout_count = 0; //reset the counter because the software suceeeded
      precent_cat = Serial.parseFloat();
      Serial.readStringUntil('\n');
    }

    current_time = millis();
  }

  if (data == false) { //if computer_vision_timeout is reached, will assume that it is a cat and judge only based on weight
    precent_cat = 1;
    cv_timeout_count++; //Computer vision failed, increment the counter
  }

}

void openning() {

  while (angle < bowl_open) { //Open the bowl incrementally until maxxium angle reached
    angle++;
    Joint1.write(angle);
    Joint2.write(angle);
  }

  is_open();
}

void is_open() {

  unsigned long start_time = millis(); //the time at which the bowl is completely opened
  unsigned long current_time = millis();
  bool cat = true; //is the cat still on the weight scale
  bool timeout = false; //has bowl_timeout occured

  while ((timeout == false) && (cat == true)) {

    weight = scale.get_units(15);
    if ((weight > cat_max) || (weight < cat_min)) { //if the weight leaves the bounds of the cat, will assume the cat is no longer there or a dog is there
      cat = false;
      bowl_timeout_count = 0; //the timeout didn't occur, reset the count
    }

    current_time = millis();
    if ((current_time - start_time) >= bowl_timeout) {
      timeout = true;
      bowl_timeout_count++; //timeout occured, increment the count
    }
  }

  closing();
}

void closing() {

  while (angle > bowl_closed) { //Open the bowl incrementally until maxxium angle reached
    angle--;
    Joint1.write(angle);
    Joint2.write(angle);
  }
}

void check_failure() {
  bool reset = false;

  if (cv_timeout_count >= 5) {
    while (reset == false) {
      digitalWrite(CV_LED, HIGH);
      delay(250);
      digitalWrite(CV_LED, LOW);
      delay(250);
    }
  }

  if (bowl_timeout_count >= 3) {
    while (reset == false) {
      digitalWrite(BOWL_LED, HIGH);
      delay(250);
      digitalWrite(BOWL_LED, LOW);
      delay(250);
    }
  }
}
