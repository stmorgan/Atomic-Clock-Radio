void scan_gps() {
  while (ss.available() > 0) gps.encode(ss.read());
            
  if (gps.date.isUpdated() && gps.time.isUpdated()) { 
    Year = gps.date.year();                
    Month = gps.date.month();
    Day = gps.date.day();
    Hour = gps.time.hour();
    Minute = gps.time.minute();
    Second = gps.time.second();
  }
  if (gps.location.isUpdated()) {
  
  }
}

