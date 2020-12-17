#include <Arduino.h>
#include <Wire.h>   
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "AS_WatchV1.h" //Include pins file!
#include <stdint.h>
#include "Icons.c"
#include <Adafruit_BME280.h>
#include <Arduino-MAX17055_Driver.h>
#include <list>
#include <TouchScreen.h>
#include <IRremote.h>


IRrecv irrecv(IR_PIN);
decode_results results;
MAX17055 bat;
Adafruit_BME280 bme;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
TouchScreen ts = TouchScreen(XM, YM, XP, YP, 380);

int hall, temp, pressure, humidity, altidute, soc, pressDuration, ChProcessStat, capicity;
int SEALEVELPRESSURE_HPA = 1013;
int lastState = LOW;
int currentState;
long long int pressedTime  = 0;
long long int releasedTime = 0;
float voltage, tte;
int toShow = 0;
bool LCD = false;
int hour,x;

void ActualizeSensors(void * parameter)
{
  for(;;){
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
    vTaskDelay(750 / portTICK_PERIOD_MS);
  }
}


void CheckButton(void * parameter)
{
  for(;;){
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

void ShowMenu()
{
  if(toShow == 0)
  {
    int BattColor;
    tft.fillRect(0, 0, 60, 34, ILI9341_BLACK);         //Hall
    tft.fillRect(70, 40, 180, 55, ILI9341_BLACK);      //Hour
    tft.fillRect(230, 0, 100, 30, ILI9341_BLACK);      //battery %
    tft.fillRect(230, 220, 90, 240, ILI9341_BLACK);   //Accel
    tft.fillRect(0, 180, 140, 230, ILI9341_BLACK);     //heigh & temp & pressure & humidity


    tft.setCursor(70, 45);      //Hour
    tft.setTextSize(6);
    tft.println("14:10"); 

    tft.drawRGBBitmap(0, 0, SettingsIcon, 30, 30);

    tft.setTextSize(2);
    tft.setCursor(230, 225);    //Accel
    tft.print("16");
    tft.println("m/s^2");


    tft.setTextSize(2);        //altidute
    tft.setCursor(0, 195); 
    tft.print(altidute);
    tft.println("m a.s.l");

    tft.setTextSize(2);        //Pressure
    tft.setCursor(0, 225);
    tft.print(pressure/100);
    tft.println("hPa");

    if(soc<100 & soc>51)
    {
      BattColor = ILI9341_GREEN;
    }
  
    else if (soc<51 & soc>21)
    {
      BattColor = ILI9341_YELLOW;
    }

    else
    {
      BattColor = ILI9341_RED;
    }
    
    if (digitalRead(ChProcess))
    {
      tft.drawRGBBitmap(280, 0, BatteryCharging, 37, 20);
    
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
      tft.drawRGBBitmap(280, 0, BatteryCharging, 37, 20);
    
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
  }
}

void ShowSettings()
{
  if(toShow == 1)
  {
    tft.drawRGBBitmap(0, 0, ReturnIcon, 30, 30);

    tft.fillRect(40, 40, 270, 170, ILI9341_BLACK);

    tft.setTextSize(2);        //Humidity
    tft.setCursor(40, 40); 
    tft.print("Humidity: ");
    tft.print(humidity);
    tft.println("%");

    tft.setTextSize(2);        //Temp
    tft.setCursor(40, 70);
    tft.print("Temp: "); 
    tft.print(temp);
    tft.setTextSize(1);
    tft.print("*");
    tft.setTextSize(2);
    tft.println("C");

    tft.setTextSize(2);         //Hall
    tft.setCursor(40, 100);
    tft.print("Hall: ");
    tft.print(hall);
    tft.setTextSize(1);
    tft.println("*");

    tft.setTextSize(2);        //Humidity
    tft.setCursor(40, 130); 
    tft.print("Voltage: ");
    tft.print(voltage);
    tft.println("V");

    tft.setTextSize(2);        //Humidity
    tft.setCursor(40, 160); 
    tft.print("Capacity: ");
    tft.print(capicity);
    tft.println("mAh");

    if(!digitalRead(ChProcess))
    {
      tft.setTextSize(2);        //Humidity
      tft.setCursor(40, 190); 
      tft.println("Charging");
    }
    else
    {
      tft.setTextSize(2);        //Humidity
      tft.setCursor(40, 190); 
      tft.print("Time to empty: ");
      tft.print(tte);
      tft.println("h");
    }

    tft.setTextSize(2);        //Humidity
    tft.setCursor(90, 220); 
    tft.print("Go to sleep");
    
    delay(200);
  }
}

void setup() 
{
  irrecv.enableIRIn();
  irrecv.blink13(true);
  analogReadResolution(10);
  tft.begin();
  tft.invertDisplay(ILI9341_INVOFF);
  bat.setCapacity(1200);
  bat.setResistSensor(0.01);
  Serial.begin(9600);
  Serial.println("WakeUp");
  bme.begin();
  pinMode(ChProcess, INPUT);
  pinMode(PanicButton, INPUT);
  pinMode(LCD_Switch, OUTPUT);
  digitalWrite(LCD_Switch, LOW);
  tft.setSPISpeed(25000000);
  tft.fillScreen(ILI9341_BLACK);
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
    ActualizeSensors,   
    "ActualizeSensors",   
    5000,            
    NULL,            
    1,               
    NULL             
    );

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 0);

}

void loop() 
{ 
  ShowMenu();
  ShowSettings();
  if (irrecv.decode(&results))
  {
      Serial.println(results.value, HEX);
      delay(10);

      switch (results.value)
      {
        case 0x76A77416:
          Serial.println("XD");
          break;

        case 0x69893291:
        Serial.println("LOL");

      }
      irrecv.resume();
  }

}