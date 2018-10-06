/*
 * == SLAVE CODE ==
 * Receive data from the master and print to Serial Monitor
 */

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10,11);  //RX,TX

int flag = 0;
 
void setup() { 
  Serial.begin(38400);
  BTSerial.begin(38400);

  Serial.println("Start up HC-05 slave module.");
}
 
void loop() {  
  // Checks whether data is coming from the serial port (HC-05)
  // Sends new data to Serial Monitor
  if (BTSerial.available() > 0) {
    
    if (flag == 0)
      Serial.println("Received data:");
    
    Serial.println(BTSerial.read());
    
    flag = 1;
  }
  
}
