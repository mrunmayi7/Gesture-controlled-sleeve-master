/*
   == VIBRATION MOTOR TEST CODE 2 ==
   Push button to simulate "incoming call" to turn on motor
   Bluetooth not yet integrated
   Switch code syntax from delay() to millis format
   Attempt at FSM????
*/

// --------- CONSTANTS ---------
// Assign pin numbers
const int motorPin = 3; // output pin for vibration motor
const int buttonPin = 2;  // input pin for push button

const int debounce = 300; // number of milliseconds between button readings to avoid debouncing


// --------- VARIABLES ---------
int buttonVal = LOW;
int callVal = LOW;
int motorState = 0;
int motorCount = 0;
int motorVal = 0;
int motorOn = 51 * 2.0;

int tapState = 0;

unsigned long motorOnVibrateInterval = 500; // millisecs motor vibration is on
unsigned long motorOnDelayInterval = 100;   // millisecs motor turns off between vibrations

unsigned long currentMillis = 0;  // stores the value of millis() in each iteration of loop()
unsigned long previousMillis = 0;
unsigned long previousMotorOn = 0;


//========================================

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(38400);
  //  BTSerial.begin(38400); // uncomment for Bluetooth integration

  Serial.println("Start up component test for motor. Push button to activate motor");
}

//========================================

void loop() {
  Serial.println("\nRESTART LOOP");

  currentMillis = millis();   // capture the latest value of millis()

  CallStateListener();  // listens for incoming call
  TapStateListener();   // listens for tap on wristband to mute call
  updateMotorStates();  // updates motor states
  switchMotor();        // actually changes motor pin states to turn on/off

}


//========================================

void CallStateListener() {

  // Only reads the button state after the button interval has elapsed
  // Avoids issues with button buoncing

  if (currentMillis - previousMillis > debounce) {
    previousMillis = currentMillis;
    
    buttonVal = digitalRead(buttonPin); // read input value from push button
    callVal = buttonVal;

    if (callVal == HIGH) {
      // Checks request to activate motor and marks flag
      motorState = 1;
      Serial.println("'Incoming Call' activated");
    }
  }
}

//========================================

void TapStateListener() {

  // Listens for tap performed on touch grid

  //  if (/* tap detected from usual gesture behavior)
  //    tapState = 1;
  //  else
  //    tapState = 0;
}

//========================================

void updateMotorStates() {

  // Based on motor state, vibrate motor
  // ***Still needs development: interrupt to turn off/mute if button is pressed again

  // vibrate motor for 500 milliseconds
  if ((callVal == HIGH) && (motorState == 1) && (tapState == 0) && (motorCount < 6)) {
    
    if (currentMillis - previousMillis > motorOnVibrateInterval) {
      // time is up, so change motorstate to low (turn off motor)
      previousMillis = currentMillis;
      
      motorState = 0; // turn off motor on next cycle
    }
  }
  
  // pause motor for 100 milliseconds between vibrations
  if ((callVal == HIGH) && (motorState == 0) && (tapState == 0) && (motorCount < 4)) {

    if (currentMillis - previousMillis > motorOnDelayInterval) {
      // time is up, so change motorstate to high (turn on motor)
      previousMillis = currentMillis;
      
      motorState = 1;
      motorCount++; // completed one cycle of vibrate
      Serial.print(motorCount);
    }
  }

}

//========================================

void switchMotor() {

  // Code that actually controls motor on and off
  if (motorState == 0)
    motorVal = 0;
  else if (motorState == 1)
    motorVal = motorOn;
   
  analogWrite(motorPin, motorVal);
  
}

