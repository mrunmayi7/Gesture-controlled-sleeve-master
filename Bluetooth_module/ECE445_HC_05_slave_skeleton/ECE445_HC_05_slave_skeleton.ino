/* 
 *  
 *  AUTHOR: Stephanie Wang
 *  ECE445, Spring 2018
 *  
 */

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX pins (that should receive TX | RX respectively)

void setup() {
//  Serial.begin(38400);  //Sets the baud for serial data transmission

  // For AT config mode, connect EN (Key) to Pin 9
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  BTSerial.begin(38400);  // HC-05 default speed in AT command mode

  Serial.println("Start up HC-05 slave module.");
  
}

void loop() {
  // put your main code here, to run repeatedly:

  // Checks whether data is coming from the serial port
  // Send data only when you receive data:
  if(Serial.available() > 0) {
    
  }

}
