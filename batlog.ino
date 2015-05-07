typedef struct {
  uint8_t logbat;
  uint16_t eeaddr;
} BatLog_t;

BatLog_t BatLog = {0};

/**
 * cookie value is written to EEPROM[0], and used to avoid overwriting
 * existing logs. To start a new log, pass in a new cookie value
 */ 
void batlog_init(uint8_t cookie) {
  BatLog.eeaddr = 1;

  if (EEPROM[0] != cookie) {
    EEPROM[0] = cookie;
    // filled the log with 0xFF so we know later when the log stops.
    for (uint16_t addr = 1; addr < 1024; ++addr) {
      EEPROM[addr] = 0xFF;
    }
    BatLog.logbat = 1;
  } else {
    Serial.begin(9600);

    oled.drawString("Press WAKE", 0, LINE_HEIGHT, FONT_SIZE, COLOR_RED);
    oled.drawString("to print batlog", 0, 0, FONT_SIZE, COLOR_RED);
    while(digitalRead(10) == HIGH);;

    for (uint16_t eeaddr = 1; eeaddr < 1024; ++eeaddr) {
      Serial.print(eeaddr, DEC);
      Serial.print(" ");
      Serial.println(EEPROM[eeaddr], DEC);
      delay(10);

      // no need to print the entire log
      if (EEPROM[eeaddr] == 0xFF) break;
    }
    delay(3000);
  }
}

void batlog_tick(uint8_t changes) {
  if (changes >= 2 && BatLog.logbat) {
    if (BatLog.eeaddr < 1024) {
      uint8_t batvol = Xadow.getBatVol()*10;
      EEPROM[BatLog.eeaddr] = batvol;
      BatLog.eeaddr += 1;
    }
  }
}
