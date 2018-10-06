/*
      8x8 LED Matrix MAX7219 Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
   Based on the following library:
   GitHub | riyas-org/max7219  https://github.com/riyas-org/max7219
*/
#include "MaxMatrix.h"
int DIN = 11;   // DIN pin of MAX7219 module
int CLK = 13;   // CLK pin of MAX7219 module
int CS = 10;    // CS pin of MAX7219 module
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
void setup() {
  m.init(); // MAX7219 initialization
  m.setIntensity(8); // initial led matrix intensity, 0-15
}
void loop() {
  // Seting the LEDs On or Off at x,y or row,column position
  /*m.setDot(6,2,true); 
  delay(1000);
  m.setDot(6,3,true);
  delay(1000);
  m.clear(); // Clears the display
  for (int i=0; i<8; i++){
    m.setDot(i,i,true);
    delay(300);
  }
  m.clear();*/
  // Displaying the character at x,y (upper left corner of the character)  

  i = 1; 

  //swipe up
  if (i == 1){
    m.writeSprite(0, 0, block);
    //delay(1000);
    for (int i=0; i<16; i++){
      m.shiftUp(false);
      delay(100);
    }
    m.clear();
  }

  //swipe down
  if (i == 2){
    m.writeSprite(0, 0, block);
    //delay(1000);
    for (int i=0; i<16; i++){
      m.shiftDown(false);
      delay(100);
    }
    m.clear();
  }

  //single tap
  if (i == 3){
    m.writeSprite(0, 0, block);
    delay(1000);
    m.clear();
    delay(1000);
  }

  //double tap
  if (i == 4){
    m.writeSprite(0, 0, block);
    delay(500);
    m.clear();
    delay(500);
    m.writeSprite(0, 0, block);
    delay(500);
    m.clear();
    delay(10000);
  }
  
  
}
