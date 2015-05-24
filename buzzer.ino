// resonant frequency of the buzzer. At this frequency the buzzer
// is the loudest
#define BUZZER_FREQ_HZ                (4000)

// standard duration of an alert
#define BUZZER_DURATION_MS            (50)

void buzzer_init() {
}

/**
 * Plays jingle number n
 */
void buzzer_jingle(uint8_t n) {
  buzzer_play(261, 100);
  buzzer_play(293, 100);
  buzzer_play(329, 100);
  buzzer_play(349, 100);
  buzzer_play(391, 100);
  buzzer_play(440, 100);
}

/**
 * Sounds the buzzer at some default frequency for some default
 * duration
 */
void buzzer_sound() {
  buzzer_play(440, BUZZER_DURATION_MS);
}

/**
 * Plays the tone specified by freq_hz for duration specified by dur_ms. Units
 * are in Hz and ms respectively.
 */ 
void buzzer_play(uint16_t freq_hz, millis_delta_t dur_ms) {
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
