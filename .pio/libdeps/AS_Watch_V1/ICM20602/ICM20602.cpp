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
