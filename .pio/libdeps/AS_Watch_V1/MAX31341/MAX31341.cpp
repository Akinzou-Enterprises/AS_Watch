#include <Arduino.h>
#include <MAX31341.h>
#include <Wire.h>

int MAX31341::begin()
{
    Wire.beginTransmission(0xD2);
    Wire.write(MAX31341_REG_CONFIG_REG1_ADDR);
    delay(5);
    Wire.write(0b0001001);
    delay(5);
    Wire.write(MAX31341_REG_CONFIG_REG2_ADDR);
    delay(5);
    Wire.write(0b0000010);
    delay(5);
    Wire.write(MAX31341_REG_CONFIG_REG2_ADDR);
    delay(10);
    Wire.write(0b0000000); 
}