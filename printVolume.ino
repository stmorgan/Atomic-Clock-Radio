void print_vol()
{
  int i; 
  lcd.setCursor(0,1);
  for (i=0; i < min(15, volume); i++) {
    lcd.print('#');
  }
}

