void button_init() {
}

/**
 * Returns 1 if the named button is pressed, 0 otherwise.
 */
uint8_t button_pressed(ButtonName name) {
  uint8_t pin = 0xff;
  uint8_t offstate = HIGH;

  if (name == BUTTON_WAKE) {
    pin = 10;
    offstate = HIGH;
  }

  if (pin == 0xff) return 0;
  else return digitalRead(pin) != offstate;
}

void button_wait(ButtonName name) {
  uint8_t pin = 0xff;
  uint8_t offstate = HIGH;

  if (name == BUTTON_WAKE) {
    pin = 10;
    offstate = HIGH;
  }

  if (pin != 0xff) while(digitalRead(pin) == offstate);
}

