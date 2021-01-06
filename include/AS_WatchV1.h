// BME280:     0x76: connected to GND,       0x77: connected to VDDIO
// MAX31341:   The address is 0xD2 (left justified with LSB set to 0).
//ICM-20600:   b1101001
//MAX17055:    0x6C

//Go to .platformio\packages\framework-arduinoespressif32\variants\esp32, open pins_arduino.h and change MOSI, MISO, SCK to: 

//static const uint8_t MOSI  = 13;
//static const uint8_t MISO  = 17;
//static const uint8_t SCK   = 14;

const uint8_t SD_CS_PIN = 5;
const uint8_t SOFT_MISO_PIN = 19;
const uint8_t SOFT_MOSI_PIN = 23;
const uint8_t SOFT_SCK_PIN  = 18;

#define SD_FAT_TYPE 1
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(10), &softSpi)
#define TFT_DC 4
#define TFT_CS 15
#define TFT_RST 27
#define LCD_Switch 2
#define PanicButton 26
#define ChProcess 25
#define IR_PIN 16
#define MaxCharsOnPage 331

#define XP 34
#define XM 36
#define YP 35
#define YM 39

#define SPIspeed 40000000L