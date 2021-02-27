#include <Arduino.h>
#include "boards/AS_WatchV1.h" //Include pins file!
#include "languages/pl.h" //Language
#include <Wire.h>   
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <stdint.h>
#include "Icons.c"
#include <Adafruit_BME280.h>
#include <Arduino-MAX17055_Driver.h>
#include <IRremote.h>
#include <SdFat.h>
#include <cstdio>
#include <cmath>
#include <MAX31341.h>
#include <lvgl.h>
#include <TFT_eSPI.h>


#define AOSversion "Alpha_0v1"
String Command = "";

TFT_eSPI tft = TFT_eSPI();
MAX31341 rtc;
SdFat32 sd;
File32 file;
IRrecv irrecv(IR_PIN);
decode_results results;
MAX17055 bat;
Adafruit_BME280 bme;
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

int column, row = 0;
int hall, temp, pressure, humidity, altidute, soc, pressDuration, ChProcessStat, capicity, minutes, hours;
int SEALEVELPRESSURE_HPA = 1013;
int lastState = LOW;
int currentState;
long long int pressedTime  = 0;
long long int releasedTime = 0;
float voltage, tte;
int toShow = 0;
bool LCD = false;
bool SD = false;
int MaxCharacters = 0;
int CharacterToShow = 1;
int pages, ActualPage = 1;
int day, month, dayOfWeek;


//functions
String ReadSerial()
{
  Command = "";
  while (Serial.available() > 0)
  {
    char read = char(Serial.read());
    if (read == ' ')
    {
      break;
    }
    Command += read;
  }
  return Command;
}



//Tasks
void ActualizeSensors(void * parameter)
{
  for(;;)
  {
    if(Command == "")
    {
      soc = bat.getSOC();
      capicity = bat.getCapacity();
      voltage = bat.getInstantaneousVoltage();
      tte = bat.getTimeToEmpty();
      hall = hallRead();   
      temp = bme.readTemperature();
      pressure = bme.readPressure();
      humidity = bme.readHumidity();
      if(bme.readAltitude(SEALEVELPRESSURE_HPA) > -9999)
      {
        altidute = bme.readAltitude(SEALEVELPRESSURE_HPA);
      }
      hours = rtc.GetHours();
      minutes = rtc.GetMinutes();
      day = rtc.GetDate();
      month = rtc.GetMonth();
      if (!rtc.GetDay())
      {
        rtc.SetDay(1);
        rtc.SetRTCData();
      }
      
      dayOfWeek = rtc.GetDay();
    }
    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}

void CheckSD(void * parameter)
{
  for(;;)
  {
    //Serial.println("Checking card");
    if (sd.begin(SD_CONFIG) && !SD) 
    {
      //Serial.println("Card in");
      if (file.open("SoftSPI.txt", O_RDONLY) && MaxCharacters == 0) 
      {
        while (file.available())
        {
          char random = file.read();
          MaxCharacters+=1;
        }
        pages = ceil(float(MaxCharacters)/MaxCharsOnPage);
        CharacterToShow -= 1;
        file.close(); 
      }
      SD = true;
    }
    else if (!sd.begin(SD_CONFIG))
    {
      //Serial.println("Card out");
      MaxCharacters = 0;
      SD = false;
    }
    
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void CheckIR(void * parameter)
{
  for(;;)
  {
    if (irrecv.decode(&results))
    {
        Serial.println(results.value, HEX);
        delay(10);

        switch (results.value)
        {
          case 0xF7807F:
            if (ActualPage > 1)
            {
              tft.fillScreen(ILI9341_BLACK);
              row = 0;
              column = 0;
              Serial.println("XD");
              ActualPage -= 1;
              break;
            }
            else if (ActualPage == 1)
            {
              ActualPage = pages;
              CharacterToShow = MaxCharacters - MaxCharsOnPage;
              break;
            }

          case 0xF700FF:
            if(MaxCharacters > 0)
            {
              if (ActualPage < pages)
              {
                tft.fillScreen(ILI9341_BLACK);
                row = 0;
                column = 0;
                Serial.println("lol");
                ActualPage += 1;
                break;
              }
              else if (ActualPage == pages)
              {
                ActualPage = 1;
                CharacterToShow = 1;
                break;
              }
            }
        }
        irrecv.resume();
    }  
    results.value = 0;
    vTaskDelay(80 / portTICK_PERIOD_MS);
  }
}

void CheckButton(void * parameter)
{
  for(;;)
  {
    currentState = digitalRead(PanicButton);
    if(lastState == HIGH && currentState == LOW)
    {
      pressedTime = millis();
    }
    else if(lastState == LOW && currentState == HIGH)
    {
      releasedTime = millis();
      pressDuration = releasedTime - pressedTime;
      Serial.println(pressDuration);

      if(pressDuration <= 1000)
      {
        if(LCD)
        {
          digitalWrite(LCD_Switch, LOW);
          LCD = false;
        }
        else
        {
          digitalWrite(LCD_Switch, HIGH);
          LCD = true;
        }
        
      }

      if( pressDuration >= 3000 && pressDuration <= 4000 )
      {
        Serial.println("Go to deep sleep");
        esp_deep_sleep_start();
      }
    }
    lastState = currentState;
    vTaskDelay(80 / portTICK_PERIOD_MS);
  }
}

void CheckVoltage(void * parameter)
{
  for(;;)
  {
    if(voltage < 3.3 & voltage != 0)
    {
      esp_deep_sleep_start();
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}




//LCD showing
void ShowMenu()
{
  if(toShow == 0)
  {
    tft.startWrite();
    int BattColor;
    tft.fillRect(0, 0, 60, 34, ILI9341_BLACK);         //Hall
    tft.fillRect(70, 40, 180, 75, ILI9341_BLACK);      //Hour
    tft.fillRect(230, 0, 100, 30, ILI9341_BLACK);      //battery %
    tft.fillRect(230, 220, 90, 240, ILI9341_BLACK);   //Accel
    tft.fillRect(0, 180, 140, 230, ILI9341_BLACK);     //heigh & pressure 


    tft.setCursor(70, 45);      //Hour
    tft.setTextSize(6);
    if(hours <= 9)
    {
      tft.print("0");
    }
    tft.print(hours);
    tft.print(":");
    if(minutes <= 9)
    {
      tft.print("0");
    }
    tft.println(minutes);
    tft.setCursor(80, 95); 
    tft.setTextSize(2);

    switch(dayOfWeek)
    {
      case 1:
        tft.print(MONDAY);
        break;

      case 2:
        tft.print(TUESDAY);
        break;

      case 3:
        tft.print(WEDNESDAY);
        break;

      case 4:
        tft.print(THURSDAY);
        break;
        
      case 5:
        tft.print(FRIDAY);
        break;

      case 6:
        tft.print(SATURDAY);
        break;

      case 7:
        tft.print(SUNDAY);
        break;
    };

    if(day <= 9)
    {
      tft.print("0");
    }
    tft
    .print(day);
    tft.print(".");

    if(month <= 9)
    {
      tft.print("0");
    }
    tft.print(month);

    tft.setTextSize(2);
    tft.setCursor(230, 225);    //Accel
    tft.print("16");
    tft.println("m/s^2");


    tft.setTextSize(2);        //altidute
    tft.setCursor(0, 195); 
    tft.print(altidute);
    tft.println(MASL);

    tft.setTextSize(2);        //Pressure
    tft.setCursor(0, 225);
    tft.print(pressure/100);
    tft.println("hPa");

    if(soc<100 & soc>51)
    {
      BattColor = ILI9341_GREEN;
    }
  
    else if (soc<=51 & soc>21)
    {
      BattColor = ILI9341_YELLOW;
    }

    else
    {
      BattColor = ILI9341_RED;
    }
    
    if (digitalRead(ChProcess))
    {

    
      tft.setTextSize(2);        
      if(soc < 100)
      {
      tft.setCursor(240, 3);
      }
      else
      {
        tft.setCursor(230, 3);
      }
      tft.print(soc);
      tft.println("%");
    }
    
    else
    {

    
      tft.setTextSize(2);        
      if(soc < 100)
      {
      tft.setCursor(240, 3);
      }
      else
      {
        tft.setCursor(230, 3);
      }
      tft.print(soc);
      tft.println("%");
    }

    tft.fillRect(282, 2, 31*soc/100, 16, BattColor);
    delay(50);
    tft.endWrite();
    
  }
}

void ShowSettings()
{
  if(toShow == 1)
  {

    tft.fillRect(40, 40, 270, 170, ILI9341_BLACK);

    tft.setTextSize(2);        
    tft.setCursor(40, 40); 
    tft.print(HUMIDITY);
    tft.print(humidity);
    tft.println("%");

    tft.setTextSize(2);        
    tft.setCursor(40, 70);
    tft.print(TEMP); 
    tft.print(temp);
    tft.setTextSize(1);
    tft.print("*");
    tft.setTextSize(2);
    tft.println("C");

    tft.setTextSize(2);         
    tft.setCursor(40, 100);
    tft.print(HALL);
    tft.print(hall);
    tft.setTextSize(1);
    tft.println("*");

    tft.setTextSize(2);        
    tft.setCursor(40, 130); 
    tft.print(VOLTAGE);
    tft.print(voltage);
    tft.println("V");

    tft.setTextSize(2);        
    tft.setCursor(40, 160); 
    tft.print(CAPACITY);
    tft.print(capicity);
    tft.println("mAh");

    if(!digitalRead(ChProcess))
    {
      tft.setTextSize(2);        
      tft.setCursor(40, 190); 
      tft.println(CHARGING);
    }
    else
    {
      tft.setTextSize(2);        
      tft.setCursor(40, 190); 
      tft.print(TIMEtoEMPTY);
      tft.print(tte);
      tft.println("h");
    }

    tft.setTextSize(2);        
    tft.setCursor(90, 220); 
    tft.print(AOSversion);
    
    delay(200);
  }
}

void ShowFromSD()
{
  if(toShow == 2 && SD && CharacterToShow <= MaxCharacters)
  { 
    if (file.open("SoftSPI.txt", O_RDONLY))
    {
      for(int i = 0; i<CharacterToShow; i++)
      {
        char skipCharacter = file.read();
      }
      if(column <= 320 & row <= 210)
      {
        delay(3);
        tft.setTextSize(2);
        char readByte = file.read();
        tft.setCursor(column, row); 
        tft.print(readByte);
        column+=13;
        if(column >= 312)
        {
          row += 16;
          column = 0;
        }
        CharacterToShow += 1;
        Serial.println(CharacterToShow);
        tft.setCursor(150, 225);
        tft.print(ActualPage);
        tft.print("/");
        tft.print(pages);
      }
      file.close();
    }
    
  }
}


//Arduino code
void setup() 
{
  rtc.begin(0b00000001);
  rtc.RTCsettings(false, V1_3, false);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  analogReadResolution(10);
  bat.setCapacity(1200);
  bat.setResistSensor(0.01);
  Serial.begin(115200);
  Serial.println("WakeUp");
  bme.begin();
  pinMode(ChProcess, INPUT);
  pinMode(PanicButton, INPUT);
  pinMode(LCD_Switch, OUTPUT);
  digitalWrite(LCD_Switch, LOW);
  lv_init();
  tft.begin();
  tft.setRotation(3);


  xTaskCreate
    (
    CheckButton,    // Function that should be called
    "CheckButton",   // Name of the task (for debugging)
    5000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    2,               // Task priority
    NULL             // Task handle
    );


  xTaskCreate
    (
    CheckSD,    
    "CheckSD",  
    2000,            
    NULL,        
    1,        
    NULL          
    );

  xTaskCreate
    (
    ActualizeSensors,   
    "ActualizeSensors",   
    5000,            
    NULL,            
    1,               
    NULL             
    );

  xTaskCreate
    (
    CheckIR,   
    "CheckIR",   
    2000,            
    NULL,            
    1,               
    NULL             
    );

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 0);

  xTaskCreate
    (
    CheckVoltage,   
    "CheckVoltage",   
    2000,            
    NULL,            
    1,               
    NULL             
    );
  
}

void loop() 
{ 
  tft.startWrite();
  tft.fillScreen(ILI9341_BLUE);
  tft.endWrite();
  delay(200);
}
