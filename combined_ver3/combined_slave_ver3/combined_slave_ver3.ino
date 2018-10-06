/* 
 *  SLAVE MODULE
 *  
 */

// --------- LIBRARIES ---------
//#include <SoftwareSerial.h>
#include "AltSoftSerial.h"
#include "CapacitiveSensor.h"


// --------- CONSTANTS ---------
// Assign pin numbers
const int motorPin = 3; // output pin for vibration motor

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);        // 10 megohm resistor between pins 4 & 5, pin 5 is sensor pin
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,7);        // 10 megohm resistor between pins 4 & 8, pin 8 is sensor pin
CapacitiveSensor   cs_4_9 = CapacitiveSensor(4,12);        // 10 megohm resistor between pins 4 & 9, pin 9 is sensor pin


AltSoftSerial BTSerial(8, 9); // RX | TX pins (that should receive TX | RX respectively)
//SoftwareSerial BTSerial(A0, A1); // RX | TX pins (that should receive TX | RX respectively)


// --------- VARIABLES ---------
int callVal = LOW;
int motorVal = 0;
int motorOn = 51 * 2.5;
//long motorOn = 77.3 * 3.3;  // IMPORTANT: ONLY USE FOR MCU ON BREADBOARD/PCB (max power 3.3V)!!!

int motorState = 0;
int pauseState = 1;
int motorCount = 0;
int motorFlag = 0;
int muteMotor = 0;

int gestureVal = 0;

unsigned long motorOnTime = 400;
unsigned long motorShortDelayTime = 200;
unsigned long motorLongDelayTime = 500;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

unsigned long motorCurrentMillis = 0;
unsigned long motorPreviousMillis = 0;

// GESTURE CONTORL VARIABLES
long touchedCutoff = 500;
long counter1 = 0;
long counter2 = 0;
long counter3 = 0;
long counter4 = 0;

long start1 = 0; 
long start2 = 0; 
long start3 = 0; 
long start7 = 0; 

long delta1 = 0;
long delta2 = 0;
long delta4 = 0; 
long delta3 = 0; 

long start4 = 0;
long start5 = 0;
long start6 = 0; 

int taps = 0;
int doubleTap = 0;

unsigned long current;
int interval = 50;

unsigned long previous = 0;

// TEST LED PINS AND VALS
//int LEDPin0 = 0;
//int LEDPin1 = 1;
int LEDPin6 = 6;
int LEDPin7 = 7;
int LEDPin12 = 12;
int LEDPin13 = 13;


//========================================

void setup() {

  Serial.begin(38400);
  BTSerial.begin(38400);  // HC-05 default speed in AT command mode

  Serial.println("Start up HC-05 slave module. Push button to activate motor.");

//  pinMode(LEDPin0, OUl

  pinMode(LEDPin6, OUTPUT);
  digitalWrite(LEDPin6, LOW);
  pinMode(LEDPin7, OUTPUT);
  digitalWrite(LEDPin7, LOW);
  
  pinMode(LEDPin12, OUTPUT);
  digitalWrite(LEDPin12, LOW);
  pinMode(LEDPin13, OUTPUT);
  digitalWrite(LEDPin13, LOW);

}

void loop() {

  current = millis();
  currentMillis = millis(); 

  BTStateListener();  // listens for data from master Bluetooth device
  updateState();

  updateMotor();
//  checkMuteMotor();
  switchMotor();
  
}

//========================================

void BTStateListener() {

  // Constantly checks for data from BT master device
  if (BTSerial.available() > 0) {
    callVal = BTSerial.read();
  }
}

//========================================


void updateState() {

  // There is no incoming call OR motor has been muted 
  if (callVal == LOW) {
    // GESTURE LOOP

    // Detect that call has ended. Reinitilaize motorflag and motorstate.
    if (motorFlag == 1) {
      Serial.println("\nEnd of call");
      motorCount = 0;
      muteMotor = 0;
    }
    motorFlag = 0;
    motorState = 0;
    muteMotor = 0;
    
    GestureDetectionLoop();  
  }
  
  // !!!!! === CURRENTLY CHANGING NEED TO RETURN TO THIS CONDITIONAL ==== !!!!!!
  // There is an incoming call AND the motor has not been muted
  else if (callVal == HIGH && muteMotor == 0) {
    // MOTOR LOOP
  
    // Detect this is the first instance of a call (not ongoing incoming call).
    if (motorFlag == 0) {
      Serial.println("\n\n'Incoming Call' detected");
      pauseState = 0;
      previousMillis = currentMillis;
    }
      
    motorFlag = 1;
    motorState = 1;
    MuteRingListener();
    
  }

  else if (callVal == HIGH && muteMotor == 1) {
    muteMotor = 1; // muteMotor until end of call detected
    motorFlag = 1;
    motorState = 0;
    
    GestureDetectionLoop();  
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
//          Serial.println("Short pause");
        }
        else if (motorCount == 5) {
          // long cycle found, turn off motor for long pause
          pauseState = 2;
          motorCount = 0;
//          Serial.println("Long pause");
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

//========================================

void MuteRingListener() {
  // Detect single tap only
  if(current - previous >= interval){
    previous = current;
//    Serial.print('\n');
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);
    long total2 =  cs_4_5.capacitiveSensor(30);
    long total3 =  cs_4_8.capacitiveSensor(30);
    long total4 =  cs_4_9.capacitiveSensor(30);

    if (total1 < touchedCutoff && total2 > touchedCutoff && total3 > touchedCutoff && total4 < touchedCutoff) {
      muteMotor = 1;
      Serial.println("Detected touch to mute the motor!");
    }
  }
}


//========================================

void GestureDetectionLoop() {
  
  if(current - previous >= interval){
    previous = current;
    Serial.print('\n');
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);
    long total2 =  cs_4_5.capacitiveSensor(30);
    long total3 =  cs_4_8.capacitiveSensor(30);
    long total4 =  cs_4_9.capacitiveSensor(30);

    Serial.print(total1);
    Serial.print("\t");
    Serial.print(total2);
    Serial.print("\t");
    Serial.print(total3);
    Serial.print("\t");
    Serial.print(total4);

    if (total1 > touchedCutoff) {
      digitalWrite(LEDPin12, HIGH);

//      Serial.println("Line 2 touched");
//      Serial.println(total1);
      
      counter1 += 10;
      start1 = millis(); 
    }
    else{
      digitalWrite(LEDPin12, LOW);
    }
  
    if (total2 > touchedCutoff){
      digitalWrite(LEDPin13, HIGH);

//      Serial.println("Line 5 touched");
//      Serial.println(total2);
      
      counter2 += 10;
      start2 = millis(); 
    }
  
    else {
      digitalWrite(LEDPin13, LOW);
    }
  
    if (total3 > touchedCutoff){
      digitalWrite(LEDPin6, HIGH);
      
//      Serial.println("Line 8 touched");
//      Serial.println(total3);
      
      counter3 += 10;
      start3 = millis(); 
    }
  
    else {
      digitalWrite(LEDPin6, LOW);
    }

    if (total4 > touchedCutoff){
      digitalWrite(LEDPin7, HIGH);
      
//      Serial.println("Line 9 touched");
//      Serial.println(total4);
      
      counter4 += 10;
      start7 = millis(); 
    }
  
    else {
      digitalWrite(LEDPin7, LOW);
    }
    
     digitalWrite(LEDPin12, LOW);
     digitalWrite(LEDPin13, LOW);
//     digitalWrite(LEDPin5, LOW);
  
     delta1 = start2 - start1;
     delta2 = start3 - start2;
     delta4 = start7 - start3; 
  

     //swipe down
     if(delta1 > 50 && delta1 < 1000 && counter1 != 0 && counter2 != 0 && counter3 != 0 && counter4 != 0 && delta2 > 50 && delta2 < 1000 && delta4 > 50 && delta4 < 1000){        
      Serial.print("\n\nSwipe Down Detected: "); // ************
      digitalWrite(LEDPin12, HIGH);
      counter1 = 0;
      counter2 = 0;
      counter3 = 0;
      counter4 = 0; 
      start1 = 0; 
      start2 = 0;
      start3 = 0;
      start7 = 0; 

      gestureVal = 1;
      BTSerial.write(gestureVal);
      gestureVal = 0; 
     }

     if(delta1 < -50 && delta1 > -1000 && counter1 != 0 && counter2 != 0 && counter3 != 0 && counter4 != 0 && delta2 < -50 && delta2 > -1000 && delta4 < -50 && delta4 > -1000){        
      Serial.print("\n\nSwipe Up Detected: "); // ************
      digitalWrite(LEDPin13, HIGH);
      counter1 = 0;
      counter2 = 0;
      counter3 = 0;
      counter4 = 0; 
      start1 = 0; 
      start2 = 0;
      start3 = 0;
      start7 = 0; 
      
      gestureVal = 2;

      BTSerial.write(gestureVal);
      gestureVal = 0;
     }

     if (taps == 1){
      start6 = millis(); 
      Serial.print("\t");
      Serial.print("start6: ");
      Serial.print(start6);

      Serial.print("\t");
      Serial.print("start4: ");
      Serial.print(start4);
      if (start6 - start4 > 500){
        Serial.print("Single Tap Detected: "); // ************
//        digitalWrite(LEDPin4, HIGH);
        taps = 0;

        gestureVal = 3;
        BTSerial.write(gestureVal);
        gestureVal = 0; 
      }
      
      
     }

     //single tap
     if (delta1 > -50 && delta1 < 50 && counter1 != 0 && counter2 != 0 && counter3 != 0 && counter4 != 0 && delta2 > -50 && delta2 < 50 && delta4 > -50 && delta4 < 50){
      if (taps == 0 && start5 != 0 && (millis() - start5 < 1000)){
           Serial.print("\t");
           Serial.print("ERROR5: ");
           Serial.print("\t");
           Serial.print("Difference5: ");
           int temp2 = millis() - start5;
           Serial.print(temp2);
           start5 = start5 + interval;
           Serial.print("\t");
           Serial.print("start5: ");
           Serial.print(start5);
      }

      else{
        taps += 1;
        if (taps == 1){
          start4 = millis(); 
          //start5 = 0;
        }
  
        if (taps == 2){
          if (millis() - start5 < 1000){
            Serial.print("\t");
            Serial.print("ERROR: ");
            int temp = millis() - start5;
            Serial.print(temp);
            start5 = start5 + interval;
            
          }
          else{
            start5 = millis(); 
            delta3 = start5 - start4;
            Serial.print("\t");
            Serial.print("delta3: ");
            Serial.print(delta3);
            if (delta3 < 500 && delta3 > 300){ // ************
              Serial.print("Double Tap Detected: ");
//              digitalWrite(LEDPin5, HIGH);
              
              gestureVal = 4;
              BTSerial.write(gestureVal);
              gestureVal = 0; 
            }
            taps = 0;
          }
          
        }
        
      }
      counter1 = 0;
      counter2 = 0;
      counter3 = 0;
      counter4 = 0;
      start1 = 0;
      start2 = 0;
      start3 = 0;
      start7 = 0; 
     }
    
   }
}
