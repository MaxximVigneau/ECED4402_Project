#include "HX711.h" //Library for Hx711 load cell amplifier and A/D converter 

#define DT 3      //Data OUT is connect to D3
#define SCLK 2    //Clock IN is connect to D2

HX711 scale;      //Declaring a object of type HX711

void setup() {

  Serial.begin(9600);

  scale.begin(DT, SCLK);
  scale.set_scale();
  scale.tare(); 

}

void loop() {
 scale.set_scale(1000);
 Serial.println(scale.get_units(10)); 

}
