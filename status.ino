#define STATUS_COLOR      (COLOR_BLUE)

void status_init() {
}

void status_show(uint8_t changes) {
  uint8_t ypos = 0;

  uint8_t batv = mcu_get_battery_voltage();
  sprintf(_sbuf, "Battery: %d.%dV", batv/10, batv%10);
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
