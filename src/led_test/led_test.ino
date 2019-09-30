#include "SparkFun_Tlc5940.h"

const byte layerpins[4] = {
  4, 2, 6, 5
};

void setup() {
  Serial.begin(115200);
  Serial.println("init");
  Tlc.init(0);//initalizes all LEDs to off at first

  for (int i = 0; i < 4; i++) {
    pinMode(layerpins[i], OUTPUT);
  }

  Serial.println("finished init");

}

void loop() {
  for (int layer = 0; layer < 4; layer++ ) {
    digitalWrite(layerpins[layer],HIGH);
    for (int led = 0; led < 48; led++) {
      Tlc.set(led,3000);
      Tlc.update();
      delay(50);
    }
    Tlc.clear();
    Tlc.update();
    digitalWrite(layerpins[layer],LOW);
  }

}
