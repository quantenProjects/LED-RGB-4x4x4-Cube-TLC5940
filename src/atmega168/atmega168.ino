#include "SparkFun_Tlc5940.h"
#include <Wire.h>

/* cube 4x4x4
  //   Atmega168
*/

// Aenderungen an der Libary:
// src/tlc_config.h:
// TLC_PWM_PERIOD auf 4096 aendern
// TLC_GSCLK_PERIOD auf 1 aendern
// Und Anzahl der TLC anpassen!

//pins
const byte layerpins[4] = {
  //4, 2, 6, 5 // das ist eigl die richtige Reihenfolge, aber richtiges debuggen ist aufwendiger als ein workaround :innocent:
  5,4,2,6
};

const byte byte_index_mapping[6] = {3,2,1,0,5,4};

// Einkommentieren um Serielle Ausgabe zu aktivieren:
//#define SerialDebug yess


byte frame[4][6] = {
  {255,0,0,0,255,0},
  {0,255,0,0,0,255},
  {255,0,63,0,255,0},
  {63,0,15,0,63,0}
  };
byte lastlayer = 0;
unsigned long start = 0;
int inverti = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("init");
  Tlc.init(0);//initalizes all LEDs to off at first

  Wire.begin(44);
  Wire.onReceive(receiveEvent);

  for (int i = 0; i < 4; i++) {
    pinMode(layerpins[i], OUTPUT);
  }

  Serial.println("finished init");

  /*for (int i = 0 ; i < 4; i++) {
    frame[0][i] = 255;
  }*/

}

void loop() {

  for (int layer = 0; layer < 4; layer++) {

    for (int byte_index = 0; byte_index < 6; byte_index++) {
      for (int bit_index = 0; bit_index < 8; bit_index++) {
        if ((frame[layer][byte_index] >> bit_index) & 1) {
          Tlc.set(byte_index_mapping[byte_index] * 8 + bit_index,4095);
        } else {
          Tlc.set(byte_index_mapping[byte_index] * 8 + bit_index,0);
        }
        #ifdef SerialDebug
        Serial.print("set led: ");
        Serial.print(byte_index_mapping[byte_index] * 8 + bit_index);
        Serial.print(" to ");
        Serial.println((frame[layer][byte_index] >> bit_index) & 1);
        #endif
      }
    }

    digitalWrite(layerpins[lastlayer], LOW);
    
    lastlayer = layer;
    while(Tlc.update());
    digitalWrite(layerpins[layer], HIGH);
    #ifdef SerialDebug
    Serial.print("layer: ");
    Serial.println(layer);
    #endif
    //delay(1);
    
  }

}

void receiveEvent(int howMany)
{
  if (howMany == 24) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 6; j++) {
        frame[i][j] = Wire.read();
      }
    }
  } else { // reset buffer
    while (Wire.available()) {
      Wire.read();
    }
  }
}
