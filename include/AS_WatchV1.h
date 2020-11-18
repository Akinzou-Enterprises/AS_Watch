// BME280:     0x76: connected to GND,       0x77: connected to VDDIO
// MAX31341:   The address is 0xD2           (left justified with LSB set to 0).
//ICM-20600:   b1101000: connected to GND,   b1101001: connected to VDDIO
//MAX17055:    0x6C                          (or 0x36 for 7 MSbit address).

//Go to .platformio\packages\framework-arduinoespressif32\variants\esp32, open pins_arduino.h and change MOSI, MISO, SCK to: 

//static const uint8_t MOSI  = 13;
//static const uint8_t MISO  = 17;
//static const uint8_t SCK   = 14;



#define TFT_DC 4
#define TFT_CS 15
#define TFT_RST 27
#define LCD_Switch 2
#define PanicButton 26
#define ChProcess 25

#define SPIspeed 40000000L