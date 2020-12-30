#pragma once

enum max31341_register_address 
{
    MAX31341_REG_CONFIG_REG1_ADDR       = 0x00,
    MAX31341_REG_CONFIG_REG2_ADDR       = 0x01,
    MAX31341_REG_INT_POLARITY_CONFIG_ADDR   = 0x02,
    MAX31341_REG_TIMER_CONFIG_ADDR      = 0x03,
    MAX31341_REG_INT_EN_REG_ADDR        = 0x04,
    MAX31341_REG_INT_STATUS_REG_ADDR    = 0x05,
    MAX31341_REG_SECONDS_ADDR           = 0x06,
    MAX31341_REG_MINUTES_ADDR           = 0x07,
    MAX31341_REG_HOURS_ADDR             = 0x08,
    MAX31341_REG_DAY_ADDR               = 0x09,
    MAX31341_REG_DATE_ADDR              = 0x0A,
    MAX31341_REG_MONTH_ADDR             = 0x0B,
    MAX31341_REG_YEAR_ADDR              = 0x0C,
    MAX31341_REG_ALM1_SEC_ADDR          = 0x0D,
    MAX31341_REG_ALM1_MIN_ADDR          = 0x0E,
    MAX31341_REG_ALM1_HRS_ADDR          = 0x0F,
    MAX31341_REG_ALM1DAY_DATE_ADDR      = 0x10,
    MAX31341_REG_ALM2_MIN_ADDR          = 0x11,
    MAX31341_REG_ALM2_HRS_ADDR          = 0x12,
    MAX31341_REG_ALM2DAY_DATE_ADDR      = 0x13,
    MAX31341_REG_TIMER_COUNT_ADDR       = 0x14,
    MAX31341_REG_TIMER_INIT_ADDR        = 0x15,
    MAX31341_REG_RAM_START_ADDR         = 0x16,
    MAX31341_REG_RAM_END_ADDR           = 0x55,
    MAX31341_REG_PWR_MGMT_REG_ADDR      = 0x56,
    MAX31341_REG_TRICKLE_REG_ADDR       = 0x57,
    MAX31341_REG_CLOCK_SYNC_REG         = 0x58,
    MAX31341_REG_END,
};

enum ClkInterput 
{
    CHz1 = 0b00,
    CHz50 = 0b01,
    CHz60 = 0b10,
    CHz32768 = 0b11,
};

enum WaveOutputFrequency 
{
    WHz1 = 0b00,
    WHz4098 = 0b01,
    WHz8192 = 0b10,
    WHz32768 = 0b11,
};

class MAX31341
{
    public:
        //if somethig what u want is not include in second begin, use this option to manualy give registers to send!
        void begin(uint8_t Config1, uint8_t Config2);

        //true to turn on, INTCN - true: Output is interrupt, false: Output is square wave
        void begin(ClkInterput ClkIn, WaveOutputFrequency WaveOutputFreq, bool Oscilator, 
                        bool ExternalClockInput, bool INTCN) ;

        void RTCsettings();
        void write8(byte reg, byte value);
        uint8_t read8(byte reg);
        void SetHour(int Hour);
        uint8_t GetHour();
};      


