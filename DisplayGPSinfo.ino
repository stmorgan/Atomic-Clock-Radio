void displayInfoGPS()
{
  if (gps.date.isValid())
  {
      int Year = gps.date.year();                 // Read time from GPS chip. 
      byte Month = gps.date.month();
      byte Day = gps.date.day();
      byte Hour = gps.time.hour();
      byte Minute = gps.time.minute();
      byte Second = gps.time.second();
      setTime(Hour, Minute, Second, Day, Month, Year);  // Set Time from GPS data string
      adjustTime(UTC_OFFSET * SECS_PER_HOUR);           // Calc current Time Zone time by offset value
    }
  else
  {
  lcd.print(F("INVALID"));
  }
  if (gps.time.isValid())
  {
  /* First Row of LCD */
   lcd.setCursor(0,0);   // Set Cursor to left-most character in first (top) row. 
      if (hourFormat12() < 10) {
     lcd.print(F(" "));
   }
   lcd.print(hourFormat12());
   lcd.print(F(":"));
   if (minute() < 10) {
     lcd.print(F("0"));
   }
   lcd.print(minute());
   lcd.print(F(" "));
   if (isAM()) {
      lcd.print(F("AM")); 
   }
   else {
      lcd.print(F("PM"));
   }
   /* Second Row of LCD */
   lcd.setCursor(0,1);   // Set Cursor to second row. 
   lcd.print(dayShortStr(weekday()));
   lcd.print(F(" "));
   lcd.print(monthShortStr(month()));
   lcd.print(F(" "));
   lcd.print(day());
   lcd.print(F("                "));
  }
  else
  {
    lcd.setCursor(0,0);
  }
}

