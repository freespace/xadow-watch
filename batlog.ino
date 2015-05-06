typedef struct {
  uint8_t logbat;
  uint16_t eeaddr;
} BatLog_t;

BatLog_t BatLog = {0};

void batlog_init() {
  BatLog.eeaddr = 1;

  if (EEPROM[0] != 0x05) {
    EEPROM[0] = 0x05;
    // filled the log with 0xFF so we know later when the log stops.
    for (uint16_t addr = 1; addr < 1024; ++addr) {
      EEPROM[addr] = 0xFF;
    }
    BatLog.logbat = 1;
  } else {
    Serial.begin(9600);

    delay(3000);
    Serial.println("Battery log detected, not overwriting");

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
