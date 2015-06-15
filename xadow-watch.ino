#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include "SGL.h"
#include "SSD1306.h"

#include "defs.h"
#include "xadow.h"

#define LEFT_GUTTER_SIZE    (FONT_X)
#define FONT_SIZE           (1)
#define LINE_HEIGHT         (FONT_SIZE*FONT_Y+4)

#define SLEEP_TIME_MS       (250)

#define REDRAW_FLAG         (0x80)

#define SCREEN_WIDTH        (128)
#define SCREEN_HEIGHT       (64)

SSD1306 oled = SSD1306(0x3C);

char *test_menu[] = {"hello", "world", "here be", "dragons", NULL};

char _sbuf[256];
char _sbuf2[64];

millis_t now = 0;
millis_t checkpoint;

typedef enum {
  OK,
  ERROR,
  ERROR_SYNC_TIMEOUT,
};

typedef enum {
  SCREEN_WATCH,
  SCREEN_STATUS,
  SCREEN_OTHER,
} Screen;

Screen currentScreen;

void setup() {
  pinMode(10, INPUT);

  Xadow.init();

  buzzer_init();
  oled.init();

  animations_boot();

  oled.fillScreen(COLOR_BLACK);

  button_init();

  buzzer_init();
  buzzer_jingle(0);

  menu_init();

  status_init();

  watch_init(now);

  // log battery once every 5 minutes, giving us >80 hr battery log
  batlog_init(0x03, 5);

  sync_init();

  oled.fillScreen(COLOR_BLACK);

  currentScreen = SCREEN_OTHER;

  oled.setBufferRegion(SSD1306_REGION_MID);
  oled.drawString("Disconnect USB", 0, 0, FONT_SIZE, COLOR_RED);
  oled.drawString("then press WAKE", 0, LINE_HEIGHT, FONT_SIZE, COLOR_RED);

  button_wait(BUTTON_WAKE);
  buzzer_sound(BUZZER_SOUND_CHIME);

  mcu_reduce_features();

  now = 0;
  checkpoint = millis();
}

void loop() {
  power_twi_disable();

  if (Xadow.getChrgState() == NOCHARGE) {
    // when on battery power disable usb and uart and
    // sleep for 200 ms
    mcu_disable_usbserial();

    Xadow.goToSleep(SLEEP_MODE_PWR_DOWN, SLEEP_TIME_MS);
    // take into account the fact millis isn't ticking over
    // during sleep
    now += SLEEP_TIME_MS;
  } else {
    // otherwise simulate sleep using delay and enable
    // usb
    mcu_enable_usbserial();
    sync_listen();

    delay(SLEEP_TIME_MS);
  }

  power_twi_enable();
  uint8_t changes = watch_tick(now);

  if (changes >= 3) buzzer_sound(BUZZER_SOUND_CHIME);

  batlog_tick(changes);

  Screen nextscreen = currentScreen;

  if (button_pressed(BUTTON_WAKE)) {
    buzzer_sound(BUZZER_SOUND_BUTTON);
    nextscreen = SCREEN_STATUS;
  } else {
    nextscreen = SCREEN_WATCH;
  }

  if (nextscreen != currentScreen) {
    oled.fillScreen(COLOR_BLACK);
    currentScreen = nextscreen;
    changes |= REDRAW_FLAG;
  }

  switch(nextscreen) {
    case SCREEN_STATUS:
      status_show(changes);
      delay(3000);
      break;

    case SCREEN_WATCH:
      watch_show(changes);
      break;
  }


  millis_t t = millis();
  now += t - checkpoint;
  checkpoint = t;
}
