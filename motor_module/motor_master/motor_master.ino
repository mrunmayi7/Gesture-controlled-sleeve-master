/*
   == VIBRATION MOTOR MASTER CODE ==
   Push button to simulate "incoming call" to turn on motor
   Using standard delay() method at the moment
   MOST BASIC CODE POSSIBLE
*/

// --------- LIBRARIES ---------
#include <SoftwareSerial.h>


// --------- CONSTANTS ---------
// Assign pin numbers
const int motorPin = 3; // output pin for vibration motor
const int buttonPin = 2;  // input pin for push button

const int debounce = 300; // number of milliseconds between button readings to avoid debouncing
const int callMaxDuration = 10000;  // maximum ring length before call redirects

SoftwareSerial BTSerial(10, 11); // RX | TX pins (that should receive TX | RX respectively)


// --------- VARIABLES ---------
int buttonVal = LOW;
int callVal = LOW;
int gestureVal = 0;

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

  currentMillis = millis();

  CallStateListener();  // listens for incoming call
  RingStateListener();
  
  BTSerial.write(callVal);  // bluetooth transmit

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

