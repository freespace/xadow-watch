#define STATUS_COLOR      (COLOR_BLUE)

void status_init() {
}

void status_show() {
  uint8_t ypos = 0;

  dtostrf(Xadow.getBatVol(), 1, 1, _sbuf2);
  sprintf(_sbuf, "Battery: %sV", _sbuf2);
  oled.drawString(_sbuf, 0, ypos, FONT_SIZE, STATUS_COLOR);

  ypos += LINE_HEIGHT;

  char *s;
  switch(Xadow.getChrgState()) {
      case NOCHARGE:
        s = "Not Charging";
        break;

      case CHARGING:
        s = "Charging";
        break;

      case CHARGDONE:
        s = "Done Charging";
        break;

       default:
         s = "UNKNOWN";
  }
  oled.drawString(s, 0, ypos, FONT_SIZE, STATUS_COLOR);

  ypos += LINE_HEIGHT;

  if (BatLog.logbat) s = "Logging Battery";
  else s = "Batt log exists";

  oled.drawString(s, 0, ypos, FONT_SIZE, STATUS_COLOR);
}