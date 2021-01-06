#include <Arduino.h>
#include <Wire.h>   
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "AS_WatchV1.h" //Include pins file!
#include <stdint.h>
#include "Icons.c"
#include <Adafruit_BME280.h>
#include <Arduino-MAX17055_Driver.h>
#include <TouchScreen.h>
#include <IRremote.h>
#include <SdFat.h>
#include <cstdio>
#include <cmath>
#include <MAX31341.h>

String Command = "";

MAX31341 rtc;
int x = 0;
void setup()
{
  Serial.begin(115200);
  rtc.begin(0b00000001);
  rtc.RTCsettings(0);
  rtc.SetSeconds(50);
  delay(700);
  rtc.SetMinutes(24);
  Serial.println(rtc.GetSeconds());
  rtc.SetDay(6);
}

String ReadSerial()
{
  Command = "";
  while (Serial.available() > 0)
  {
    char read = char(Serial.read());
    if (read == ' ')
    {
      break;
    }
    Command += read;
  }
  return Command;
}

void loop()
{
  ReadSerial();

  if (Command == "A")
  {
    Serial.print("Connected!");
  }

  if (Command == "A0")
  {
    ReadSerial();
    rtc.SetSeconds(atoi(Command.c_str()));
    ReadSerial();
    rtc.SetMinutes(atoi(Command.c_str()));
    rtc.SetRTCData();
  }

  if (Command == "A3")
  {
    Serial.println(rtc.GetSeconds());
    Serial.println(rtc.GetMinutes());
  }
      
    
}