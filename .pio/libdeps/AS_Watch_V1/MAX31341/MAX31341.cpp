#include <Arduino.h>
#include <MAX31341.h>
#include <Wire.h>
#define BIN2BCD(val) ((((val) / 10) << 4) + (val) % 10)
#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)

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



void MAX31341::SetSeconds(int Seconds)
{
    write8(MAX31341_REG_SECONDS_ADDR, BIN2BCD(Seconds));
    SetRTCData();
}


uint8_t MAX31341::GetSeconds()
{
    uint8_t Data = BCD2BIN(read8(MAX31341_REG_SECONDS_ADDR));
    return Data;
}

void MAX31341::SetMinutes(int Minutes)
{
    write8(MAX31341_REG_MINUTES_ADDR, BIN2BCD(Minutes));
    SetRTCData();
}


uint8_t MAX31341::GetMinutes()
{
    uint8_t Data = BCD2BIN(read8(MAX31341_REG_MINUTES_ADDR));
    return Data;
}

void MAX31341::SetHours(int Hours)
{
    write8(MAX31341_REG_HOURS_ADDR, BIN2BCD(Hours));
    SetRTCData();
}


uint8_t MAX31341::GetHours()
{
    uint8_t Data = BCD2BIN(read8(MAX31341_REG_HOURS_ADDR));
    return Data;
}

void MAX31341::SetDay(int Days)
{
    write8(MAX31341_REG_DAY_ADDR, BIN2BCD(Days));
    SetRTCData();
}


uint8_t MAX31341::GetDay()
{
    uint8_t Data = BCD2BIN(read8(MAX31341_REG_DAY_ADDR));
    return Data;
}

void MAX31341::SetMonth(int Month)
{
    write8(MAX31341_REG_MONTH_ADDR, BIN2BCD(Month));
    SetRTCData();
}


uint8_t MAX31341::GetMonth()
{
    uint8_t Data = BCD2BIN(read8(MAX31341_REG_MONTH_ADDR));
    return Data;
}

void MAX31341::SetYear(int Year)
{
    write8(MAX31341_REG_YEAR_ADDR, BIN2BCD(Year));
    SetRTCData();
}


uint8_t MAX31341::GetYear()
{
    uint8_t Data = BCD2BIN(read8(MAX31341_REG_YEAR_ADDR));
    return Data;
}

void MAX31341::SetDate(int Date)
{
    write8(MAX31341_REG_DATE_ADDR, BIN2BCD(Date));
    SetRTCData();
}


uint8_t MAX31341::GetDate()
{
    uint8_t Data = BCD2BIN(read8(MAX31341_REG_DATE_ADDR));
    return Data;
}

void MAX31341::SetRTCData()
{
    ConfigToSend = read8(MAX31341_REG_CONFIG_REG2_ADDR);
    ConfigToSend &= ~(6);
    ConfigToSend = read8(MAX31341_REG_CONFIG_REG2_ADDR);
    ConfigToSend &= ~(6);
    write8(MAX31341_REG_CONFIG_REG2_ADDR, ConfigToSend);
    ConfigToSend |= 2;
    write8(MAX31341_REG_CONFIG_REG2_ADDR, ConfigToSend);
    delay(15);
    ConfigToSend &= ~(2);
    write8(MAX31341_REG_CONFIG_REG2_ADDR, ConfigToSend);
    ConfigToSend |= 4;
    write8(MAX31341_REG_CONFIG_REG2_ADDR, ConfigToSend);
}

void MAX31341::ReadData(byte reg)
{
    ConfigToSend = read8(MAX31341_REG_CONFIG_REG2_ADDR);
    ConfigToSend |= 4;
    write8(reg, ConfigToSend);
    ConfigToSend &= ~(4);
    ConfigToSend = read8(MAX31341_REG_CONFIG_REG2_ADDR);
    write8(reg, ConfigToSend);
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