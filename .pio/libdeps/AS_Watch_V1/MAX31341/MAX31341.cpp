#include <Arduino.h>
#include <MAX31341.h>
#include <Wire.h>

uint8_t MAX31341Adress = 0x69;

void MAX31341::begin()
{
    Wire.begin();
}

void MAX31341::SetHour(int Hour)
{
    write8(MAX31341_REG_HOURS_ADDR, Hour);
}

uint8_t MAX31341::GetHour()
{
    uint8_t Hour = read8(MAX31341_REG_HOURS_ADDR);
    return Hour;
}

void MAX31341::write8(byte reg, byte value) 
{
  Wire.beginTransmission((uint8_t)MAX31341Adress);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}

uint8_t MAX31341::read8(byte reg) 
{
    uint8_t value;
    Wire.beginTransmission((uint8_t)MAX31341Adress);
    Wire.write((uint8_t)reg);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)MAX31341Adress, (byte)1);
    value = Wire.read();
    Serial.println(value);
    return value;
}