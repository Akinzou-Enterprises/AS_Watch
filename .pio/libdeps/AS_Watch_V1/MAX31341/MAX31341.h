//Before set time set RD and SET in config 2 to 0 and change SET to 1 when u upload values, wait 10ms and change it to 0
//Before reading set RD to 1

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
    CHz1 = 0b00 << 4,
    CHz50 = 0b01 << 4,
    CHz60 = 0b10 << 4,
    CHz32768 = 0b11 << 4,
};

enum WaveOutputFrequency 
{
    WHz1 = 0b00 << 1,
    WHz4098 = 0b01 << 1,
    WHz8192 = 0b10 << 1,
    WHz32768 = 0b11 << 1,
};

enum BREF
{
    V1_3 = 0b00 << 4, 
    V1_7 = 0b01 << 4,
    V2 = 0b10 << 4,
    V2_2 = 0b11 << 4,
};

class MAX31341
{
    public:
        //if somethig what u want is not include, use this option to manualy give registers to send!
        void begin(uint8_t Config1);
        void RTCsettings(uint8_t Config2);

        //true to turn on, INTCN - true: Output is interrupt, false: Output is square wave
        void begin(ClkInterput ClkIn, WaveOutputFrequency WaveOutputFreq, bool Oscilator, bool ExternalClockInput, bool INTCN) ;
        void RTCsettings(bool SetRTC, bool ReadRTC, bool I2Ctimeout, BREF voltage, bool DataRetend);
        void ReadData(byte reg);
        void SetRTCData();
        void write8(byte reg, byte value);
        void reset();
        uint8_t read8(byte reg);


        void SetSeconds(int Seconds);
        uint8_t GetSeconds();

        void SetMinutes(int Minutes);
        uint8_t GetMinutes();

        void SetHours(int Hours);
        uint8_t GetHours();

        void SetDay(int Days);
        uint8_t GetDay();

        void SetMonth(int Month);
        uint8_t GetMonth();

        void SetYear(int Year);
        uint8_t GetYear();

        void SetDate(int Date);
        uint8_t GetDate();
        
};      


