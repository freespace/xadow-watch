// resonant frequency of the buzzer. At this frequency the buzzer
// is the loudest
#define BUZZER_FREQ_HZ                (4000)

// standard duration of an alert
#define BUZZER_DURATION_MS            (250)

void buzzer_init() {
}

/**
 * Plays jingle number n
 */
void buzzer_jingle(uint8_t n) {
  buzzer_sound(261, 100);
  buzzer_sound(293, 100);
  buzzer_sound(329, 100);
  buzzer_sound(349, 100);
  buzzer_sound(391, 100);
  buzzer_sound(440, 100);
}

/**
 * Sounds the buzzer. If freq_hz is 0 then the centre frequency
 * of the piezo is used. 
 *
 * If dur_ms is 0, then a default duration of 250 ms is used.
 */ 
void buzzer_sound(uint16_t freq_hz, millis_delta_t dur_ms) {
  if (freq_hz == 0) freq_hz = BUZZER_FREQ_HZ;
  if (dur_ms == 0) dur_ms = BUZZER_DURATION_MS;

  // we need to end SPI b/c we use MISO to control
  // the buzzer
  SPI.end();

  // make MISO an output
  DDRB |= _BV(PB3);

  millis_t now = millis();

  // generate a 2 kHz tone using delay. Not elegant but close enough
  uint16_t period_us = 1000000/freq_hz;

  while (millis() - now < dur_ms) {
    PORTB |= _BV(PB3);
    delayMicroseconds(period_us/2);
    PORTB &= ~_BV(PB3);
    delayMicroseconds(period_us/2);
  }

  // restart SPI or OLED will not work
  SPI.begin();
}
