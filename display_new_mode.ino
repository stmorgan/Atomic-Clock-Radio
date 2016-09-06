void display_new_mode()
{

  lcd.setCursor(0,0); 

  switch (mode) {
    case NORMAL:
    {
      lcd.print(F("NORMAL    "));
      // if volume increment/decrement buttons pressed, adjust volume. 
      // if radio change buttons pressed, change channel and update displayed channel. 
      
    }
    break;
  case ALARMSET:
    {
      lcd.print(F("ALARMSET  "));
      // adjust time for alarm for weekday and weekend, turn alarms on and off. 
    }
    break;
  case GMT_OFFSET:
    {
      lcd.print(F("GMT_OFFSET"));
      // check for GMT offset increment/decrement and adjust offset. (-12 wraps around to 12). 
    }
    break;
  case LAT_LONG:
    {
      lcd.print(F("LAT_LONG  "));
      // read lat/long. and print to lcd
    }
    break;
  case STN_INFO:
    {
      lcd.print(F("STN_INFO  "));
    }
    break;
   default: mode = NORMAL; 
   break; 
  } // End of switch(mode)
  lcd.setCursor(0,1);
  lcd.print(F("Mode "));
  lcd.print(mode);
  lcd.print(F("          "));

  delay(500);  // Wait long enough to see new mode. 
}
