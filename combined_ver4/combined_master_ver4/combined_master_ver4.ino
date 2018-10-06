/*
   == MASTER CODE ==
   
   *****Still needs code for matrix LED
*/

// --------- LIBRARIES ---------
//#include <SoftwareSerial.h>
#include "AltSoftSerial.h"
#include "MaxMatrix.h"


// --------- CONSTANTS ---------
const int motorPin = 3; // output pin for vibration motor
const int buttonPin = 2;  // input pin for push button

const int debounce = 300; // number of milliseconds between button readings to avoid debouncing
const int callMaxDuration = 10000;  // maximum ring length before call redirects

const int DIN = 11;   // DIN pin of MAX7219 module
const int CLK = 13;   // CLK pin of MAX7219 module
const int CS = 10;    // CS pin of MAX7219 module

AltSoftSerial BTSerial(8, 9); // RX | TX pins (that should receive TX | RX respectively)


// --------- VARIABLES ---------
int buttonVal = LOW;
int callVal = LOW;
int gestureVal = 0;
int prevGesture = 0;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int i = 0; 
int maxInUse = 1;

MaxMatrix m(DIN, CS, CLK, maxInUse); 

char block[] = {8, 8,
                  B11111111,
                  B11111111,
                  B11111111,
                  B11111111,
                  B11111111,
                  B11111111,
                  B11111111,
                  B11111111
                 };


//========================================

void setup() {
  //  // For AT config mode, connect EN (Key) to Pin 9
//  pinMode(9, OUTPUT);
//  digitalWrite(9, HIGH);

  Serial.begin(38400);
  BTSerial.begin(38400);  // HC-05 default speed in AT command mode

  Serial.println("Start up HC-05 master module. Push button to activate motor.");

  m.init(); // MAX7219 initialization
  m.setIntensity(8); // initial led matrix intensity, 0-15

}

void loop() {

//  BTSerial.write(callVal);

  currentMillis = millis(); 

  SleeveStateListener();
  

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
        updateLEDMatrix();
      }
    }
    prevGesture = gestureVal;
  }
  
}

//========================================

void updateLEDMatrix() {
  //swipe up
  if (gestureVal == 2){
    m.writeSprite(0, 0, block);
//    delay(1000);
    for (int i=0; i<16; i++){
      m.shiftUp(false);
      delay(50);
    }
    m.clear();
  }

  //swipe down
  if (gestureVal == 1){
    m.writeSprite(0, 0, block);
//    delay(1000);
    for (int i=0; i<16; i++){
      m.shiftDown(false);
      delay(50);
    }
    m.clear();
  }

  //single tap
  if (gestureVal == 3){
    m.writeSprite(0, 0, block);
    delay(1000);
    m.clear();
//    delay(1000);
  }

  //double tap
  if (gestureVal == 4){
    m.writeSprite(0, 0, block);
    delay(500);
    m.clear();
    delay(500);
    m.writeSprite(0, 0, block);
    delay(500);
    m.clear();
//    delay(10000);
  }
  
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
