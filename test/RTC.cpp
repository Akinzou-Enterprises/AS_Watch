#include <Arduino.h>
#include <Wire.h>

void setup()
{
    Serial.begin();
    uint8_t value;
    Wire.beginTransmission((uint8_t)0x69);
    Wire.write((uint8_t)0x2);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)0x69, (byte)1);
    value = Wire.read();
}