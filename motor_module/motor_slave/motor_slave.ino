/*
   == VIBRATION MOTOR SLAVE CODE ==
   Receive data from master on whether or not there is an incoming call
   Using standard delay() method at the moment
   MOST BASIC CODE POSSIBLE
*/

// --------- LIBRARIES ---------
#include <SoftwareSerial.h>


// --------- CONSTANTS ---------
// Assign pin numbers
const int motorPin = 3; // output pin for vibration motor
const int buttonPin = 2;  // input pin for push button

SoftwareSerial BTSerial(10, 11); // RX | TX pins (that should receive TX | RX respectively)


// --------- VARIABLES ---------
int callVal = LOW;
int motorVal = 0;
int motorOn = 51 * 2.0;
//long motorOn = 77.3 * 3.3;  // ONLY USE FOR MCU ON BREADBOARD/PCB (max power 3.3V)
int motorState = 0;
int pauseState = 1;
int motorCount = 0;
int motorFlag = 0;
int muteMotor = 0;

unsigned long motorOnTime = 400;
unsigned long motorShortDelayTime = 200;
unsigned long motorLongDelayTime = 500;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

unsigned long motorCurrentMillis = 0;
unsigned long motorPreviousMillis = 0;


//========================================

void setup() {

//  // For AT config mode, connect EN (Key) to Pin 9
//  pinMode(9, OUTPUT);
//  digitalWrite(9, HIGH);

  Serial.begin(38400);
  BTSerial.begin(38400);  // HC-05 default speed in AT command mode

  Serial.println("Start up HC-05 slave module. Push button to activate motor.");

}

void loop() {
  
  currentMillis = millis(); 
  
  BTStateListener();  // listens for data from master Bluetooh device
  updateState();
  updateMotor();
  switchMotor();

}

//========================================

void BTStateListener() {

  // Constantly checks for data from BT master device
  if (BTSerial.available() > 0) {
    callVal = BTSerial.read();
//    Serial.println(callVal);
  }
}

//========================================


void updateState() {

  // There is no incoming call OR motor has been muted 
  if ((callVal == LOW)) {
    
    // Detect that call has ended. Reinitilaize motorflag and motorstate.
    if (motorFlag == 1) {
      Serial.println("End of call");
      motorCount = 0;
    }
    motorFlag = 0;
    motorState = 0;
  }

  // There is an incoming call AND motor has not been muted
  else if ((callVal == HIGH)) {
    
    // Detect this is the first instance of a call (not ongoing incoming call).
    if (motorFlag == 0) {
      Serial.println("\n\n'Incoming Call' detected");
      pauseState = 0;
      previousMillis = currentMillis;
    }
    motorFlag = 1;
    motorState = 1;
  }
  
}

//========================================

void updateMotor() {
  
  if (motorState == 1) {
  
    // vibrate motor for 500 milliseconds
    if (pauseState == 0) {
      
      if (currentMillis - previousMillis > motorOnTime) {
        // time is up, so change pausestate accordingly (turn off motor for long or short pause)
        previousMillis = currentMillis;
        pauseState = 1;
        
        if (motorCount < 5) {
          // short cycle found, turn off motor for short pause
          pauseState = 1;
          Serial.println("Short pause");
        }
        else if (motorCount == 5) {
          // long cycle found, turn off motor for long pause
          pauseState = 2;
          motorCount = 0;
          Serial.println("Long pause");
        }
      }
    }
    
    // pause motor for 100 milliseconds (short pause)
    if (pauseState == 1) {

      if (currentMillis - previousMillis > motorShortDelayTime) {
        // time is up, so change pausestate accordingly (turn on motor)
        previousMillis = currentMillis;
        
        pauseState = 0;
        motorCount++; // completed one single cycle of vibrate + short pause
      }
    }

    // pause motor for 500 milliseconds (long pause)
    if (pauseState == 2) { 
    
      if (currentMillis - previousMillis > motorLongDelayTime) {
        previousMillis = currentMillis;
        
        pauseState = 0;
        motorCount = 0; // reinitialize cycle count for vibrate + short pause
      }
    }
  }
  
}

//========================================

void switchMotor() {

  if (pauseState == 0)
    analogWrite(motorPin, motorOn);
  if ((pauseState == 1) || (pauseState == 2) || (motorState == 0))
    analogWrite(motorPin, 0);
  
}

