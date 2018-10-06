/*
 * == VIBRATION MOTOR TEST CODE ==
 * Push button to simulate "incoming call" to turn on motor
 * Bluetooth not yet integrated
 */

// --------- CONSTANTS ---------
// Assign pin numbers
const int motorPin = 3; // output pin for vibration motor
const int pushPin = 2;  // input pin for push button

// --------- VARIABLES ---------
int pushVal = LOW;
int motorFlag = 0;
int motorCount = 0;
int motorOn = 51*2.0;

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(pushPin, INPUT);
  
  Serial.begin(38400);
//  BTSerial.begin(38400); // uncomment for Bluetooth integration

  Serial.println("Start up component test for motor. Push button to activate motor");
}

void loop() {
  Serial.println("\nRESTART");
  delay(5000);
  
  pushVal = digitalRead(pushPin); // read input value from push button
  Serial.print("Current pushVal = ");
  delay(1000);
  Serial.println(pushVal);
  
  if (pushVal == HIGH)
    Serial.println("Button was pushed");
  else
    Serial.println("Button was not pushed");

  // Checks request to activate motor and marks flag
  // if pushVal input is HIGH (button released)
  if (pushVal == HIGH) {
    motorFlag = 1;
    Serial.println("'Incoming Call' activated");
  }

  if (motorFlag == 1) {
    // Vibrate motor 5 times and interrupt to turn off/mute if button is pressed again
    motorCount = 0;
    do {
      analogWrite(motorPin, motorOn);
      delay(500);
      
      analogWrite(motorPin, 0);
      delay(100);
      motorCount++;
      Serial.print(motorCount);
    } while (motorCount < 5);
//    } while ((motorCount < 5) && (pushVal != HIGH));

    // Turn off motor until next activation and set flag back to 0
    analogWrite(motorPin, 0);
    
    motorFlag = 0;
  }
  else if (motorFlag == 0) {
    analogWrite(motorPin, 0);
  }

}
