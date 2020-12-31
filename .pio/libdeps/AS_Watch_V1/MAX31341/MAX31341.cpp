#include <Arduino.h>
#include <MAX31341.h>
#include <Wire.h>

uint8_t MAX31341Adress = 0x69;
uint8_t ConfigToSend;

void MAX31341::begin(uint8_t Config1)
{
    Wire.begin();
    write8(MAX31341_REG_CONFIG_REG1_ADDR, Config1);
}

void MAX31341::RTCsettings( uint8_t Config2)
{
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
        ConfigToSend |= CHz1;
        break;
    
    case CHz50:
        ConfigToSend |= CHz50;
        break;
    case CHz60:
        ConfigToSend |= CHz60;
        break;

    case CHz32768:
        ConfigToSend |= CHz32768;
        break;
    }

    switch(WaveOutputFreq)
    {
    case WHz1:
        ConfigToSend |= WHz1;
        break;
    
    case WHz4098:
        ConfigToSend |= WHz4098;
        break;
    case WHz8192:
        ConfigToSend |= WHz8192;
        break;

    case WHz32768:
        ConfigToSend |= WHz32768;
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

void MAX31341::RTCsettings(bool SetRTC, bool ReadRTC, bool I2Ctimeout, BREF voltage, bool DataRetend)
{
    ConfigToSend = 0b00000000;
    if (SetRTC)
    {
        ConfigToSend |= 0b1 << 1;
    }

    if (ReadRTC)
    {
        ConfigToSend |= 0b1 << 2;
    }
    
    if(I2Ctimeout)
    {
        ConfigToSend |= 0b1 << 3;
    }

    switch(voltage)
    {
        case V1_3:
            ConfigToSend |= V1_3;
            break;
        
        case V1_7:
            ConfigToSend |= V1_7;
            break;
        
        case V2:
            ConfigToSend |= V2;
            break;
        
        case V2_2:
            ConfigToSend |= V2_2;
            break;
    }

    if(DataRetend)
    {
        ConfigToSend |= 0b1 << 6;
    }
    
    write8(MAX31341_REG_CONFIG_REG2_ADDR, ConfigToSend);
}

void MAX31341::reset()
{
    write8(MAX31341_REG_CONFIG_REG1_ADDR, 0b00000000);
    delay(10);
    write8(MAX31341_REG_CONFIG_REG1_ADDR, 0b00000001);
}

void MAX31341::SetHour(int Hour)
{
    SetData(MAX31341_REG_HOURS_ADDR, Hour);
}

uint8_t MAX31341::GetHour()
{
    uint8_t Hour = read8(MAX31341_REG_HOURS_ADDR);
    return Hour;
}

void MAX31341::SetData(byte reg, byte value)
{
    ConfigToSend = read8(MAX31341_REG_CONFIG_REG2_ADDR);
    ConfigToSend &= ~(2);
    write8(MAX31341_REG_CONFIG_REG2_ADDR, ConfigToSend);
    Serial.println(read8(MAX31341_REG_CONFIG_REG2_ADDR));
    write8(reg, value);
    ConfigToSend |= 0b1 << 1;
    write8(MAX31341_REG_CONFIG_REG2_ADDR, ConfigToSend);
    Serial.println(read8(MAX31341_REG_CONFIG_REG2_ADDR));
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
    return value;
}