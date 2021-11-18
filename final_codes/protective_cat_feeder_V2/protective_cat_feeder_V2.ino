#include "pcf_header.h"

/*Global Varibles*/
float weight = 0;              //weight read in from scale
float percent_cat = 0;         //value returned from the computer vision program
int angle = 0;                 //current location of the servo motors (0 == closed, 90 == open)
int cv_timeout_count = 0;      //Tracks the number of times the computer vision software has failed to return a value in a row
int bowl_timeout_count = 0;    //Tracks the number of times the bowl time out has occured in a row

void setup() {

  /* Initializing Serial Communication */
  Serial.begin(9600);

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

  /*Check Scale*/
  weight = scale.get_units(15);

  if (weight > cat_min) {
    computer_vision();

    if ((weight < cat_max) && (percent_cat > 0.75)) {
      openning();
    }
  }

  check_failure();
}


void computer_vision() {

  unsigned long start_time;
  unsigned long current_time;
  bool data = false; //has the computer vision program returned a value
  percent_cat = 0;

  Serial.println("Check Camera"); //tell Rasberry Pi to run image classifer

  start_time = millis(); //miliseconds passed since the program started when image classifer started
  current_time = millis();

  while (((current_time - start_time) < computer_vision_timeout) && data == false) {

    if (Serial.available() > 0) {
      data = true;
      cv_timeout_count = 0; //reset the counter because the software suceeeded
      percent_cat = Serial.parseFloat();
      Serial.readStringUntil('\n');
    }

    current_time = millis();
  }

  if (data == false) { //if computer_vision_timeout is reached, will assume that it is a cat and judge only based on weight
    percent_cat = 1;
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
