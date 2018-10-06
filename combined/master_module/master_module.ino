/*
   == MASTER CODE ==
   
   *****Still needs code for matrix LED
*/

// --------- LIBRARIES ---------
#include <SoftwareSerial.h>


// --------- CONSTANTS ---------
const int motorPin = 3; // output pin for vibration motor
const int buttonPin = 2;  // input pin for push button

const int debounce = 300; // number of milliseconds between button readings to avoid debouncing
const int callMaxDuration = 10000;  // maximum ring length before call redirects

SoftwareSerial BTSerial(10, 11); // RX | TX pins (that should receive TX | RX respectively)


// --------- VARIABLES ---------
int buttonVal = LOW;
int callVal = LOW;
int gestureVal = 0;
int prevGesture = 0;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;


//========================================

void setup() {
  //  // For AT config mode, connect EN (Key) to Pin 9
//  pinMode(9, OUTPUT);
//  digitalWrite(9, HIGH);

  Serial.begin(38400);
  BTSerial.begin(38400);  // HC-05 default speed in AT command mode

  Serial.println("Start up HC-05 master module. Push button to activate motor.");

}

void loop() {

//  BTSerial.write(callVal);

  currentMillis = millis(); 

  SleeveStateListener();
//  updateLEDMatrix();

  CallStateListener();  // listens for incoming call
  RingStateListener();
  
  BTSerial.write(callVal);  // bluetooth transmit

}

//========================================

void SleeveStateListener() {
  
  // Constantly checks for data from BT slave device (sleeve module)
  if (BTSerial.available() > 0) {
    gestureVal = BTSerial.read();
    
    // print to serial to check
    if (gestureVal == 1 || gestureVal == 2 || gestureVal == 3 || gestureVal == 4) {
      if (gestureVal != 0) {
        Serial.print("New gesture detected: ");
        Serial.println(gestureVal);
      }
    }
    prevGesture = gestureVal;
  }
  
}

//========================================

void updateLEDMatrix() {
  // coming soon!!!!!!
}

//========================================

void CallStateListener() {

  // Only reads the button state after the button interval has elapsed
  // Avoids issues with button buoncing

  buttonVal = digitalRead(buttonPin); // read input value from push button
    
  if ((buttonVal == HIGH) && (callVal == LOW)) {
    callVal = HIGH;
    previousMillis = currentMillis; // Mark time call was detected
    Serial.println("\n\n'Incoming Call' detected.");
    Serial.println(callVal);
  }
    
}

//========================================

void RingStateListener() {

  if ((callVal == HIGH) && (currentMillis - previousMillis > callMaxDuration)) {
    callVal = LOW;
    Serial.println("\nReached max ring length. End of call.");
    Serial.println(callVal);
  }
}


