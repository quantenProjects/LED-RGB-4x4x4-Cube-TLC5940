// RGB-LED-Cube 4x4x4
// Atmega328 für SD-Slot

#include <SD.h>
#include <Wire.h>
#include <SPI.h>
File myFile_1;
File myFile_2;
File myFile_3;

int delayms = 10;

byte push =0;

byte led_1 = 7; // Blau
byte led_2 = 8; // Grün
byte led_3 = 9; // Rot

void setup() {
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);  
  Wire.begin();
  blinken(25, 3);
  delay(50);
  pinMode(10, OUTPUT); //10 = cs pin
  if (!SD.begin(10)) { // 10 = cs pin
    while (true) {
      blinken(600, 3);
      //Fehler SD Karte
    }
  }
  
  blinken(150, 3);

  myFile_1 = SD.open("1.cab", FILE_WRITE); //file with the ani data. cab = cuba animator binary ;) because of the 8.3 file names not longer file extension are allowed
  if ((myFile_1.size() % 24) != 0) {
    while (true) {
      ; //because than the file bad and have not the rigth length
    }
  }

   myFile_2 = SD.open("2.cab", FILE_WRITE); //file with the ani data. cab = cuba animator binary ;) because of the 8.3 file names not longer file extension are allowed
  if ((myFile_2.size() % 24) != 0) {
    while (true) {
      ; //because than the file bad and have not the rigth length
    }
  }

   myFile_3 = SD.open("3.cab", FILE_WRITE); //file with the ani data. cab = cuba animator binary ;) because of the 8.3 file names not longer file extension are allowed
  if ((myFile_3.size() % 24) != 0) {
    while (true) {
      ; //because than the file bad and have not the rigth length
    }
  }
  
  blinken(500, 1);
}

void loop() {

  Wire.beginTransmission(44);
  for (int i = 0; i < 12; i++) {
          if(push == 0){Wire.write(myFile_1.read());}
     else if(push == 1){Wire.write(myFile_2.read());}
     else if(push == 255){Wire.write(myFile_3.read());}
  }
  Wire.endTransmission();
  
  delay(1); //delay 10 ms that the slave can compute the i2c input and dont overfill the buffer
  
  Wire.beginTransmission(44);
  for (int i = 0; i < 12; i++) {
          if(push == 0){Wire.write(myFile_1.read());}
     else if(push == 1){Wire.write(myFile_2.read());}
     else if(push == 255){Wire.write(myFile_3.read());}
  }
  //blinken(1,5);
  //  Wire.write(132); //daten ist ein array mit den daten
  int resultint = Wire.endTransmission();
  if (resultint != 0) {
    //rum das es nicht geht zB mit LED
    blinken(1, 5);
    delay(5);
  }
  
if(push == 0){ if (myFile_1.available() == 0) {
              myFile_1.seek(0);
              blinken(1,10);
    
              Wire.requestFrom(44, 2);// request 2 bytes from slave device #44
              delay(1);
              delayms = Wire.read(); 
              push = Wire.read();
           }}
if(push == 1){ if (myFile_2.available() == 0) {
              myFile_2.seek(0);
              blinken(1,10);
    
              Wire.requestFrom(44, 2);// request 2 bytes from slave device #44
              delay(1);
              delayms = Wire.read(); 
              push = Wire.read();
           }}
if(push == 255){ if (myFile_3.available() == 0) {
              myFile_3.seek(0);
              blinken(1,10);
    
              Wire.requestFrom(44, 2);// request 2 bytes from slave device #44
              delay(1);
              delayms = Wire.read(); 
              push = Wire.read();
           }}

//  Wire.read();
  delay(delayms);
  
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
