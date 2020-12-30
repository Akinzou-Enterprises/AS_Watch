#include <Arduino.h>
#include <MAX31341.h>
#include <Wire.h>

uint8_t MAX31341Adress = 0x69;
uint8_t ConfigToSend;

void MAX31341::begin(uint8_t Config1, uint8_t Config2)
{
    write8(MAX31341_REG_CONFIG_REG1_ADDR, Config1);
    write8(MAX31341_REG_CONFIG_REG2_ADDR, Config2);
}

void MAX31341::begin(ClkInterput ClkIn, WaveOutputFrequency WaveOutputFreq, bool Oscilator, 
                        bool ExternalClockInput, bool INTCN)                   
{
    Wire.begin();
    ConfigToSend = 0b00000001;
    
    switch(ClkIn)
    {
    case CHz1:
        ConfigToSend |= CHz1 << 4;
        break;
    
    case CHz50:
        ConfigToSend |= CHz50 << 4;
        break;
    case CHz60:
        ConfigToSend |= CHz60 << 4;
        break;

    case CHz32768:
        ConfigToSend |= CHz32768 << 4;
        break;
    }

    switch(WaveOutputFreq)
    {
    case WHz1:
        ConfigToSend |= WHz1 << 1;
        break;
    
    case WHz4098:
        ConfigToSend |= WHz4098 << 1;
        break;
    case WHz8192:
        ConfigToSend |= WHz8192 << 1;
        break;

    case WHz32768:
        ConfigToSend |= WHz32768 << 1;
        break;
    }

    if(Oscilator)
    {
        ConfigToSend |= 0b0 << 3;
    }

    else
    {
        ConfigToSend |= 0b1 << 3;
    }   
    
    if(ExternalClockInput)
    {
        ConfigToSend |= 0b1 << 7;
    }

    else
    {
        ConfigToSend |= 0b1 << 7; 
    }
    
    if(INTCN)
    {
        ConfigToSend |= 0b1 << 6; 
    }

    else
    {
        ConfigToSend |= 0b0 << 6; 
    }
    write8(MAX31341_REG_CONFIG_REG1_ADDR, ConfigToSend);
}

void MAX31341::RTCsettings()
{

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