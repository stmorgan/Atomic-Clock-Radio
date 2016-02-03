#include <Time.h>
#include <TimeLib.h>

#include <LiquidCrystal.h>
#include <LCDKeypad.h>

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Si4703_Breakout.h>
#include <Wire.h>

/* Radio Setup */
int resetPin = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);
int channel;
int volume;
char rdsBuffer[10];

/* GPS Setup */
static const int RXPin = 11, TXPin = 10;
static const uint32_t GPSBaud = 9600;

static const int UTC_OFFSET = -8;  // Greenwich Mean Time offset 

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// Liquid Crystal Setup

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
 
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT; 
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}

void setup()
{
  /* Radio Setup */
  Serial.begin(9600);
  Serial.println("\n\nSi4703_Breakout Test Sketch");
  Serial.println("===========================");  
  Serial.println("a b     Favourite stations");
  Serial.println("+ -     Volume (max 15)");
  Serial.println("u d     Seek up / down");
  Serial.println("r       Listen for RDS Data (15 sec timeout)");
  Serial.println("Send me a command letter.");
  

  radio.powerOn();
  radio.setVolume(0);


  /* GPS Setup */
  // Serial.begin(9600);
  ss.begin(GPSBaud); 

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

// LCD Keypad Setup
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
  
} // End of Setup

void loop()
{
  serviceRadio();
  serviceGPS();
 // delay(1000); // Wait one second.
}// End of Main Loop

void displayInfo()
{
   Serial.print("Channel:"); Serial.print(channel); 
   Serial.print(" Volume:"); Serial.println(volume); 
}

void serviceGPS()
{
 // Serial.println("GPS Stub");

 while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfoGPS();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void  serviceRadio()
{
if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'u') 
    {
      channel = radio.seekUp();
      displayInfo();
    } 
    else if (ch == 'd') 
    {
      channel = radio.seekDown();
      displayInfo();
    } 
    else if (ch == '+') 
    {
      volume ++;
      if (volume == 16) volume = 15;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == '-') 
    {
      volume --;
      if (volume < 0) volume = 0;
      radio.setVolume(volume);
      displayInfo();
    } 
    else if (ch == 'a')
    {
      channel = 881; // Rock FM
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'b')
    {
      channel = 953; // BBC R4
      radio.setChannel(channel);
      displayInfo();
    }
    else if (ch == 'r')
    {
      Serial.println("RDS listening");
      radio.readRDS(rdsBuffer, 15000);
      Serial.print("RDS heard:");
      Serial.println(rdsBuffer);      
    }
  }
  
}

void displayInfoGPS()
{

  // int hour;

  // Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {

      int Year = gps.date.year();
      byte Month = gps.date.month();
      byte Day = gps.date.day();
      byte Hour = gps.time.hour();
      byte Minute = gps.time.minute();
      byte Second = gps.time.second();



        // Set Time from GPS data string
        setTime(Hour, Minute, Second, Day, Month, Year);
        // Calc current Time Zone time by offset value
        
         Serial.println();
         Serial.print("Before: "); 
         Serial.print(hour());
        adjustTime(UTC_OFFSET * SECS_PER_HOUR);     
         Serial.print(" After: "); 
         Serial.println(hour());
        
    
  Serial.print(gps.date.month());
  Serial.print(F("/"));
  Serial.print(gps.date.day());
    lcd.setCursor(0,0);
   // lcd.print(gps.date.month());
   // lcd.print(F("/"));
   // lcd.print(gps.date.day());
   lcd.print(month());
   lcd.print(F("/"));
   lcd.print(day());

   
  }
  else
  {
    Serial.print(F("INVALID"));
    lcd.print(F("INVALID"));
  }

  Serial.print(F(" "));
  lcd.print(F(" "));
  if (gps.time.isValid())
  {
    //hour = gps.time.hour(); 
   //if (hour > 12) hour = hour - 12; 
    //if (hour == 0) hour = 12; 
   // if (hour < 10) Serial.print(F(" "));
   // Serial.print(hour);
   // lcd.print(hour);
    
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());

    // lcd.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
   // lcd.print(gps.time.minute());
   // lcd.print(F(":"));
   // if (gps.time.second() < 10) lcd.print(F("0"));
   // lcd.print(gps.time.second());

   lcd.print(hour());
   lcd.print(F(":"));
   lcd.print(minute());
   lcd.print(F(" "));

    
  }
  else
  {
    Serial.print(F("INVALID"));
    lcd.setCursor(0,0);
    lcd.print(F("INVALID"));
  }

  Serial.println();
}

