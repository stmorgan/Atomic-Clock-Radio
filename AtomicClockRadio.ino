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

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


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

  int hour; 
  
  

  // Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    hour = gps.time.hour(); 
    if (hour > 12) hour = hour - 12; 
    if (hour == 0) hour = 12; 
    if (hour < 10) Serial.print(F(" "));
    Serial.print(hour);
    
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

