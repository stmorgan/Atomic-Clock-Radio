#include <Time.h>
#include <TimeLib.h>

#include <LiquidCrystal.h>
#include <LCDKeypad.h>

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Si4703_Breakout.h>
#include <Wire.h>


#define NORMAL 0              // Mode labels.
#define ALARMSET 1
#define GMT_OFFSET 2
#define LAT_LONG 3
#define STN_INFO 4

int mode = NORMAL;  // Stores the display and input state. Default is NORMAL, shows time, date and station. 



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

static const int UTC_OFFSET = -7;  // Greenwich Mean Time offset 

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
 // LCD Keypad Setup
 lcd.begin(16, 2);              // start the library
 lcd.clear();                   // Clear display
 lcd.setCursor(0,0);
 
  /* Radio Setup */
  radio.powerOn();
  radio.setVolume(8);
  channel = 881;                // Set CBC as default.
  radio.setChannel(channel);
  displayRadioInfo();           // Print radio channel 

  /* GPS Setup */
  ss.begin(GPSBaud); 
} // End of Setup

void loop()
{
char str_channel[5];
float flt_channel; 
  
/** Set up state machine for keyboard buttons. 
 *  States:  Main - shows time and date, shows whether there is an active alarm. 
 *                - allows for changing the radio station. 
 *           Alarm set - for setting the alarm. 
 *           Lat/long - Shows the latitude and longitude. 
 */
//  serviceRadio();
//  serviceGPS();

//lcd.setCursor(0,0); 
//lcd.print(GMT_OFFSET);

lcd.setCursor(0,1);
lcd.print(mode);

switch (mode) {
  case NORMAL:
  {
//    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(F("NORMAL    "));
    // if volume increment/decrement buttons pressed, adjust volume. 
    // if radio change buttons pressed, change channel and update displayed channel. 
    
  }
  break;
case ALARMSET:
  {
//    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(F("ALARMSET "));
    // adjust time for alarm for weekday and weekend, turn alarms on and off. 
  }
  break;
case GMT_OFFSET:
  {
//    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(F("GMT_OFFSET"));
    // check for GMT offset increment/decrement and adjust offset. (-12 wraps around to 12). 
  }
  break;
case LAT_LONG:
  {
//    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(F("LAT_LONG "));
    // read lat/long. and print to lcd
  }
  break;
case STN_INFO:
  {
//    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(F("STN_INFO "));
  }
  break;
 default: mode = NORMAL; 
 break; 
} // End of switch(mode)

////lcd.clear();
//lcd.setCursor(0,0); 
//lcd.print(mode);
delay(10);


// Check if the mode ("Select") button has been pressed, and if so, move to the next mode. s
lcd_key = read_LCD_buttons();
if (lcd_key == btnSELECT) mode = (mode + 1) % 5; // cycle through modes when select pressed.
do {                                        // Wait for key to be released. 
    lcd_key = read_LCD_buttons();
    delay(10);
  } while (lcd_key != btnNONE);


 }// End of Main Loop





