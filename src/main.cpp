#include <Arduino.h>
#include "boards/AS_WatchV1.h" //Include pins file!
#include "languages/pl.h" //Language
#include <Wire.h>   
#include <SPI.h>
#include <stdint.h>
#include "Icons.c"
#include <Adafruit_BME280.h>
#include <Arduino-MAX17055_Driver.h>
#include <IRremote.h>
#include <SdFat.h>
#include <cstdio>
#include <cmath>
#include <MAX31341.h>
#include <TFT_eSPI.h>
#include <BluetoothSerial.h>
#include <ICM20602.h>

ICM20602 ICM(true);

BluetoothSerial SerialBT;
String message = "";
int counter = 0;
int position[3];
char incomingChar;

void setup() 
{
  ICM.Restart();
  ICM.Init(AutoSelect, false, false, false);
  ICM.GyroS(true, true, true);
  Serial.begin(9600);
  SerialBT.begin("ESP32");
  Serial.println("The dev50ice started, now you can pair it with bluetooth!");
  ICM.GyroPrecision(dps250);
  Serial.println(ICM.read8(GYRO_CONFIG));
  ICM.SetOffsetGyroX(300);
  Serial.println(ICM.read8(XG_OFFS_USRL));
  Serial.println(ICM.read8(XG_OFFS_USRH));

}

double artan;

void loop() 
{ 
  Serial.println(ICM.ReadGyroX());
  delay(100);
}
