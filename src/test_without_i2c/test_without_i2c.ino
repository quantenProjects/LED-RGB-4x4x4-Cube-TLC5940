#include <avr/pgmspace.h>

#include "SparkFun_Tlc5940.h"

/* cube 4x4x4
 //   Atmega168
*/

//pins
const byte layerpins[4] = {
  4, 2, 6, 5
};

const boolean layerdriveroffstate = false;
const int framedelay = 900;

//-------------------------------------------------------------------------   
PROGMEM const byte frames[1][4][6] = {
  { {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},
    {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},
    {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},
    {B11111111, B00000000, B11110000, B11110000, B11110000, B00001111}
  },
};
const int framecount = 43;
//------------------------------------------------------------------------

byte frame[4][6];
byte activeframe = 0;
byte lastlayer = 0;
unsigned long start = 0;
int inverti = 0;


void setup() {
Tlc.init(0);//initalizes all LEDs to off at first

Serial.begin(9600);


  for (int i = 0; i < 4; i++) {
    pinMode(layerpins[i], OUTPUT);
  }
  
}

void loop() {

  for (int i = 0; i < 4; i++) {


    for(int i=0;i<48;i++){
      Tlc.set(i,WERT);
    }
 
    digitalWrite(layerpins[lastlayer], LOW);
    digitalWrite(layerpins[i], HIGH);
    lastlayer = i;
    delayMicroseconds(10);
    

    Tlc.update();

  }

  if ((millis() - start) > framedelay) {
    start = millis();
    Serial.println(start);
    activeframe++;
    if (activeframe > framecount) {
      activeframe = 0;
    }
    Serial.println(activeframe);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 6; j++) {
        //Serial.print(i);
        // Serial.print(",");
        //Serial.println(j);

        frame[i][j] = pgm_read_byte(&(frames[activeframe][i][j]));
        //Serial.println(frame[i][j]);
      }
    }
    //Serial.println("");
  }

}
