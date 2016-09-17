void print_vol()
{
  int i, k;
  lcd.setCursor(0,1);
  for (i=0; i < min(15, volume); i++) {
    lcd.print('#');
  }
for (k=i; k < 15; k++) {
    lcd.print(' ');
  }
}

