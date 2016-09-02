void displayRadioInfo() {
    lcd.setCursor(11,0);  
  if (channel < 1000) {
    lcd.print(F(" "));
    // lcd.setCursor(12,0);
  }
  lcd.print(channel / 10);                    // Print integer part of FM frequency.
  lcd.print(F("."));                          // Print decimal point.
  lcd.print(channel - 10 * (channel / 10));   // Print decimal digit in FM frequency
}                                             // End of displayRadioInfo

