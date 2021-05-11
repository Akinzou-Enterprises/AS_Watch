#include <ICM20602.h>

ICM20602::ICM20602(bool Addr)
{ 
    if(Addr)
    {
        Addr = 0x69;
    }
    
    else
    {
        Addr = 0x68;
    }
    Wire.begin();
}

void ICM20602::Restart()
{
    write8(PWR_MGMT_1, 0x80);
    delay(10);
}

void::ICM20602::Init(CLKSEL CLK, bool EnableTemp, bool GryroStandby, bool Cycle)
{
    uint8_t ConfigToSend = 0b00000000;
    switch (CLK)
    {
    case Internal20MHz:
        ConfigToSend |= Internal20MHz;
        break;
    
    case AutoSelect:
        ConfigToSend |= AutoSelect;
        break;

    case StopClock:
        ConfigToSend |= StopClock;
    }

    if(EnableTemp)
    {
        ConfigToSend |= 0x8;
    }

    if(GryroStandby)
    {
        ConfigToSend |= 0x10;
    }

    if(Cycle)
    {
        ConfigToSend |= 0x20;
    }
    
    write8(PWR_MGMT_1, ConfigToSend);
}


void ICM20602::GyroStandby(bool On)
{
    uint8_t ConfigToSend = read8(PWR_MGMT_1);
    if(On)
    {
        ConfigToSend |= 0x10;
    }

    else
    {
        ConfigToSend &= ~(0x10);
    }
    write8(PWR_MGMT_1, ConfigToSend);
}

bool ICM20602::Test()
{
    if (read8(WHO_AM_I) == 0x12)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

void ICM20602::Sleep(bool On)
{
    uint8_t ConfigToSend = read8(PWR_MGMT_1);
    if(On)
    {
        ConfigToSend |= 0x40;
    }
    
    else
    {
        ConfigToSend &= ~(0x40);
    }
    write8(PWR_MGMT_1, ConfigToSend);
}


void ICM20602::write8(byte reg, byte value) 
{
  Wire.beginTransmission((uint8_t)Addr);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}

uint8_t ICM20602::read8(byte reg) 
{
    uint8_t value;
    Wire.beginTransmission((uint8_t)Addr);
    Wire.write((uint8_t)reg);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)Addr, (byte)1);
    value = Wire.read();
    return value;
}
