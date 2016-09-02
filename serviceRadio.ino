void  serviceRadio()
{

lcd_key = read_LCD_buttons();
switch (lcd_key){
  case btnNONE:
  {
    break;
  }
  case btnRIGHT:
  {
    channel = channel + 2;                // Increase frequency.
    if (channel > 1080) channel = 881;    // Make sure channel is in broadcast range (wrap around dial).
    radio.setChannel(channel);
    displayRadioInfo();                          // Print radio channel 
    break;
  }
    case btnLEFT:
  {
    channel = channel - 2;              // Decrease frequency. 
    if (channel < 880) channel = 1079;  // Make sure channel is in broadcast range (wrap around dial). 
    radio.setChannel(channel);
    displayRadioInfo();                          // Print radio channel 
    break;
  }
    case btnUP:
  { 
    volume ++;
    if (volume == 16) volume = 15;
    radio.setVolume(volume);
    print_vol();
    break;
  }
    case btnDOWN:
  {
    volume --;
    if (volume < 0) volume = 0;
    radio.setVolume(volume);
    print_vol();
    break;
  }
    case btnSELECT:
  {
    mode = ALARMSET; 
    
    break;
  }
}

do {                                        // Wait for key to be released. 
    lcd_key = read_LCD_buttons();
  } while (lcd_key != btnNONE);
}  // Asertion: lcd_key == btnNONE

