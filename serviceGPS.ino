
void serviceGPS()
{
 while (ss.available() > 0)
    if (gps.encode(ss.read()))
    //  displayInfoGPS();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
   lcd.setCursor(0,0);
   lcd.print(F("No GPS detected"));
   while(true);
  }
}  // End of serviceGPS()

