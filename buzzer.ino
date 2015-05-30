// resonant frequency of the buzzer. At this frequency the buzzer
// is the loudest
#define BUZZER_FREQ_HZ                (4000)

// standard duration of an alert
#define BUZZER_DURATION_MS            (100)

#define D_NOTE                        (587)
#define E_NOTE                        (659)
#define G_NOTE                        (783)
#define A_NOTE                        (880)
#define B_NOTE                        (987)
#define REST_NOTE                     (0)

void buzzer_init() {
}

/**
 * Plays jingle number n
 */
void buzzer_jingle(uint8_t n) {
  uint8_t p = 100;
  buzzer_play(D_NOTE, p);
  buzzer_play(E_NOTE, p);
  buzzer_play(G_NOTE, p);
  buzzer_play(A_NOTE, p);
  buzzer_play(B_NOTE, p);
}

/**
 * Sounds the buzzer in response to buttons
 */
void buzzer_sound(BuzzerSound sound) {
  if (sound == BUZZER_SOUND_BUTTON) {
    buzzer_play(E_NOTE, BUZZER_DURATION_MS);
  } else if (sound == BUZZER_SOUND_CHIME) {
    buzzer_play(B_NOTE, BUZZER_DURATION_MS);
    buzzer_play(REST_NOTE, BUZZER_DURATION_MS);
    buzzer_play(B_NOTE, BUZZER_DURATION_MS);
  }
}

/**
 * Sounds the 

/**
 * Plays the tone specified by freq_hz for duration specified by dur_ms. Units
 * are in Hz and ms respectively.
 */ 
void buzzer_play(uint16_t freq_hz, millis_delta_t dur_ms) {
  if (freq_hz == 0) delay(dur_ms);
  else {
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
}
