typedef struct {
  uint8_t logbat;
  uint16_t eeaddr;
} BatLog_t;

BatLog_t BatLog = {0};

// TODO: implement battery life prediction based on batlog, automatically
// start new log every 10 charge cycles to keep prediction accurate.

/**
 * cookie value is written to EEPROM[0], and used to avoid overwriting
 * existing logs. To start a new log, pass in a new cookie value
 */
void batlog_init(uint8_t cookie) {
  BatLog.eeaddr = 1;

  if (EEPROM[0] != cookie) {
    oled.drawString("Making new batlog", 0, 0, FONT_SIZE, COLOR_RED);
    EEPROM[0] = cookie;
    // filled the log with 0xFF so we know later when the log stops.
    for (uint16_t addr = 1; addr < 1024; ++addr) {
      EEPROM[addr] = 0xFF;
    }
    BatLog.logbat = 1;
  } else if (Xadow.getChrgState() != NOCHARGE) {
    // if we are connected to USB then print charge log
    mcu_enable_usbserial();

    uint8_t ypos = 0;
    oled.drawString("Press WAKE", 0, ypos, FONT_SIZE, COLOR_RED);
    ypos += LINE_HEIGHT;
    oled.drawString("to print batlog", 0, ypos, FONT_SIZE, COLOR_RED);
    ypos += LINE_HEIGHT;

    millis_t startt = millis();
    uint8_t printlog = 1;

    Serial.println("Press WAKE to print batlog");

    while(digitalRead(10) == HIGH) {
      millis_delta_t diff = millis() - startt;
      if (diff> 3000) {
        printlog = 0;
        break;
      } else {
        oled.drawString("\x80", 0, ypos, FONT_SIZE, COLOR_BLACK);
        sprintf(_sbuf, "%d", 3-diff/1000);
        oled.drawString(_sbuf, 0, ypos, FONT_SIZE, COLOR_RED);
        delay(250);
      }
    }

    if (printlog) {
      delay(100);
      for (uint16_t eeaddr = 1; eeaddr < 1024; ++eeaddr) {
        Serial.print(eeaddr, DEC);
        Serial.print(" ");
        Serial.println(EEPROM[eeaddr], DEC);
        delay(10);

        // no need to print the entire log
        if (EEPROM[eeaddr] == 0xFF) break;
      }
    }
  }
}

void batlog_tick(uint8_t changes) {
  if (changes >= 2 && BatLog.logbat) {
    if (BatLog.eeaddr < 1024) {
      uint8_t batvol = mcu_get_battery_voltage();
      EEPROM[BatLog.eeaddr] = batvol;
      BatLog.eeaddr += 1;
    }
  }
}
