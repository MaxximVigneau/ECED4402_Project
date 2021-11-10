/* File: unidirectional_serial_communication_test
 * Programmer: Maxxim Vigneau 
 * Purpose: To establish one way serial communication
 *          from an Arduino nano clone to a 
 *          Rasberry pi 4 
 * Notes: Ensure to go to tools and select ATmega328P 
 *        (Old Bootloder)
 * Date: NOV 9 2021
 */


void setup() {

  Serial.begin(9600); //initalizes serial communication and sets the baud rate to 9600bps 

}

void loop() {

  Serial.println("Sent from Arduino Nano"); 
  delay(1000); //waits 1 second 

}
