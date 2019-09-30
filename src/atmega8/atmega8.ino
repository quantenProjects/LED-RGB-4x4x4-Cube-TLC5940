#include <Wire.h>
#include <avr/pgmspace.h>
#include <SPI.h>

// RGB-LED-Cube 4x4x4
// Atmega 8

const byte layerpins[4] = {
//  2, 3, 4, 5   // Layer vertauscht EB0=EB3 & EB3=EB0
    5, 4, 3, 2
};
const byte latchpin = 9;
/*  Connect:
    Serial in on MOSI
    Clock on SCK  */
    
const int poti = 15; // Geschwindigkeit

//false then the layerdriver will set LOW when they have to be off - true then the layerdriver will set HIGH when they have to be off
const boolean layerdriveroffstate = false;

boolean seconddata = false;

byte frame[4][6];
byte activeframe = 0;
byte lastlayer = 0;
unsigned long start = 0;
int inverti = 0;

byte led_1 = 6; // Blau
byte led_2 = 7; // Grün
byte led_3 = 8; // Rot

#include "OneButton.h"
const int buttonPin = A0;
OneButton button(buttonPin, false);
byte push;


void setup() {
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(poti, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  button.attachClick(clickedIt);
  button.attachDoubleClick(doubleClickedIt);
  button.attachLongPressStart(longPressStart1);
  
  Wire.begin(44);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  SPI.begin();
  pinMode(latchpin, OUTPUT);
  
  for (int i = 0; i < 4; i++) {
    pinMode(layerpins[i], OUTPUT);
  }
  blinken(100, 5); 
  
  for (int i = 0 ; i < 4; i++) {
    frame[0][i] = 255;
  }
  blinken(2, 20);
}

void loop() {
  button.tick();

  for (int i = 0; i < 4; i++) {

    //for (int j = 0;j<8;j++) {

    //SPI.transfer(frame[i][j]);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Info I know that this is bad programming, but I think that is faster than a for loop
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    SPI.transfer(frame[i][0]);
    SPI.transfer(frame[i][1]);
    SPI.transfer(frame[i][2]);
    SPI.transfer(frame[i][3]);
    SPI.transfer(frame[i][4]);
    SPI.transfer(frame[i][5]);

    //}
    digitalWrite(layerpins[lastlayer], LOW);
    // digitalWrite(layerpins[lastlayer],layerdriveroffstate);
    digitalWrite(latchpin, HIGH);
    //digitalWrite(layerpins[i],!(layerdriveroffstate));
    digitalWrite(layerpins[i], HIGH);
    lastlayer = i;
    delayMicroseconds(800);
    digitalWrite(latchpin, LOW);

  }


}

void receiveEvent(int howMany)
{
  //here may be a comando to reset secondval to false if any i2c packet is lost an the cube display chaos
  //blinken(1,20);
  if (howMany == 12) {
    int startval = 0;
    if ( ! seconddata) { //first data
      startval = 0;
    } else { //second data
      startval = 2;
    }
    for (int i = startval; i < (startval + 2); i++) {
      for (int j = 0; j < 6; j++) {
        frame[i][j] = Wire.read();
      }
    }
    seconddata = ! seconddata;
  }
}

void requestEvent(){
  Wire.write(analogRead(poti)/ 4); // ERST AUF /4
  //Wire.write(10);
  Wire.write(push);
}

void blinken(int del, int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(led_2, HIGH);
    delay(del);
    digitalWrite(led_2, LOW);
    delay(del);
  }
}

void blinken2(int del2, int n2) {
  for (int i2 = 0; i2 < n2; i2++) {
    digitalWrite(led_1, HIGH);
    delay(del2);
    digitalWrite(led_1, LOW);
    delay(del2);
  }
}



void clickedIt() {
delay(50);
blinken2(100, 1);
push = 0;
}
void doubleClickedIt() {
delay(50);
blinken2(150, 2);
push = 1;
}

void longPressStart1() {
delay(50);
blinken2(800, 1);
push = 255;
} 
