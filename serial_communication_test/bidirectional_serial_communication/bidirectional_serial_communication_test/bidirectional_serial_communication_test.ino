/* File: bidirectional_serial_communication_test
   Programmer: Maxxim Vigneau
   Purpose: To establish to way communication
            between Arduino Nano Clone and
            Raspberry Pi 4
   Date: NOV 9 2021
*/

void setup() {

  Serial.begin(9600); //Initalize Serial Communication, and set baud rate to 9600bps

}

void loop() {

  if (Serial.available() > 0) {                 //Check to see if any bytes have already arrived and are stored in the buffer 
    
    String data = Serial.readStringUntil('\n'); //Read the data in to a string variable until a newline charater is entered 
    Serial.print("You sent me: ");              //Send the Recived message back 
    Serial.println(data);
    
  }

}
