/* Description: To test absolute touch on two capsense pins using a common receive pin.
  The file outputs RC time constant values (need to confirm with scope) to the serial monitor. 
  These are the return value outputted by capsense library function
  
  TO DETECT SWIPE:
  Based on testing data, @ swipe speed 33.33 cm/s, the delta bet. 4 lines placed 2.8 cm away from each other
  has a time delta = 84 ms. So for a swipe range of +- 10% range (92.4 - 75.6 ms): 
  minimum time bet. signal lines for a swipe is 75 ms
  maximum time bet. signal lines for a swipe is 92.4 ms
 
  pseudo-code:
  initialize counters to 0
  loop:
  check if line 1 has been touched:
    if yes, start counter1 (increment every 10 ms) / if no, break
      for 70 < counter1 < 100:                            \\this can be a function (args -> line# AND return -> true/false)
      check if line2 has been touched:
        if yes, reset counter1 to 0 (increment every 10 ms)
        if no, break.
        for 70 < counter1 < 100:
          check if line3 has been touched:
            if yes, reset counter1 to 0, SWIPE DETECTED!
            if no, break.
*/
#include <CapacitiveSensor.h>

/*
 * Uses a high value resistor e.g. 10 megohm between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
 */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);        // 10 megohm resistor between pins 4 & 5, pin 5 is sensor pin
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);        // 10 megohm resistor between pins 4 & 8, pin 8 is sensor pin
CapacitiveSensor   cs_4_9 = CapacitiveSensor(4,9);        // 10 megohm resistor between pins 4 & 9, pin 9 is sensor pin

int LEDPin = 13;
int LEDPin1 = 12;
int LEDPin2 = 11;
int LEDPin3 = 10;
int LEDPin4 = 7;
int LEDPin5 = 6;
int LEDPin6 = 3; 

long touchedCutoff = 1000;
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

void setup()                    
{

 //  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(38400); 
   // The baud rate affects the counter value transmission, ie. it takes more time for the LEDpin1 to turn on at slower baud rates. 
    //At 57600, it took a little less than 3 seconds for counter = 300 at 1 millis delay
   pinMode(LEDPin, OUTPUT);
   digitalWrite(LEDPin, LOW);
   pinMode(LEDPin1, OUTPUT);
   digitalWrite(LEDPin1, LOW);
   pinMode(LEDPin2, OUTPUT);
   digitalWrite(LEDPin2, LOW);
   pinMode(LEDPin3, OUTPUT);
   digitalWrite(LEDPin3, LOW);
   pinMode(LEDPin4, OUTPUT);
   digitalWrite(LEDPin4, LOW);
   pinMode(LEDPin5, OUTPUT);
   digitalWrite(LEDPin5, LOW);
   pinMode(LEDPin6, OUTPUT);
   digitalWrite(LEDPin6, LOW);
}


unsigned long previous = 0;

void loop()                    
{
  
  unsigned long current = millis();
  int interval = 100;
  
  if(current - previous >= interval){
    previous = current;
    Serial.print('\n');
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);
    long total2 =  cs_4_5.capacitiveSensor(30);
    long total3 =  cs_4_8.capacitiveSensor(30);
    long total4 =  cs_4_9.capacitiveSensor(30);

    if (total1 > touchedCutoff) {
      digitalWrite(LEDPin, HIGH);
      counter1 += 10;
      start1 = millis(); 
    }
    else{
      digitalWrite(LEDPin, LOW);
    }
  
    if (total2 > touchedCutoff){
      digitalWrite(LEDPin1, HIGH);
      counter2 += 10;
      start2 = millis(); 
    }
  
    else {
      digitalWrite(LEDPin1, LOW);
    }
  
    if (total3 > touchedCutoff){
      digitalWrite(LEDPin2, HIGH);
      counter3 += 10;
      start3 = millis(); 
    }
  
    else {
      digitalWrite(LEDPin2, LOW);
    }

    if (total4 > touchedCutoff){
      digitalWrite(LEDPin6, HIGH);
      counter4 += 10;
      start7 = millis(); 
    }
  
    else {
      digitalWrite(LEDPin6, LOW);
    }
    
     digitalWrite(LEDPin3, LOW);
     digitalWrite(LEDPin4, LOW);
     digitalWrite(LEDPin5, LOW);
  
     delta1 = start2 - start1;
     delta2 = start3 - start2;
     delta4 = start7 - start3; 
  

     //swipe down
     if(delta1 > 50 && delta1 < 1000 && counter1 != 0 && counter2 != 0 && counter3 != 0 && counter4 != 0 && delta2 > 50 && delta2 < 1000 && delta4 > 50 && delta4 < 1000){        
      Serial.print("Swipe Down Detected: ");
      digitalWrite(LEDPin3, HIGH);
      counter1 = 0;
      counter2 = 0;
      counter3 = 0;
      counter4 = 0; 
      start1 = 0; 
      start2 = 0;
      start3 = 0;
      start7 = 0; 
     }

     if(delta1 < -50 && delta1 > -1000 && counter1 != 0 && counter2 != 0 && counter3 != 0 && counter4 != 0 && delta2 < -50 && delta2 > -1000 && delta4 < -50 && delta4 > -1000){        
      Serial.print("Swipe Up Detected: ");
      digitalWrite(LEDPin3, HIGH);
      counter1 = 0;
      counter2 = 0;
      counter3 = 0;
      counter4 = 0; 
      start1 = 0; 
      start2 = 0;
      start3 = 0;
      start7 = 0; 
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
        Serial.print("Single Tap Detected: ");
        digitalWrite(LEDPin4, HIGH);
        taps = 0;
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
            if (delta3 < 500 && delta3 > 300){
              Serial.print("Double Tap Detected: ");
              digitalWrite(LEDPin5, HIGH);
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
 //end of loop interval
 // 1) fails to account for consecutive taps on the same wire. i.e. 2 taps on wire1, wire2, and wire3 would classify as a swipe as long as deltas are still small
 
}


