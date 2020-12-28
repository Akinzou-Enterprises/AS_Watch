#include <Arduino.h>
#include <MAX31341.h>
#include <Wire.h>

void MAX31341::begin()
{
    write8(MAX31341_REG_CONFIG_REG1_ADDR, 0b0001001);
    write8(MAX31341_REG_CONFIG_REG2_ADDR, 0b0000010);
    delay(10);
    write8(MAX31341_REG_CONFIG_REG2_ADDR, 0b0000000);
}

void MAX31341::write8(byte reg, byte value) 
{
    Wire.begin((uint8_t)I2CAddress);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

uint8_t MAX31341::read8(byte reg) 
{
    uint8_t value;
    Wire.beginTransmission((uint8_t)I2CAddress);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)I2CAddress, (byte)1);
    value = Wire.read();
    return value;
}