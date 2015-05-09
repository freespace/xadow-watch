#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include "SGL.h"
#include "SSD1331.h"

#include "xadow.h"

// These are arduino pin definitions. Note that on leonardo
// boards MOSI and sclk cannot be directly used, and hence
// D16 and D15 is a thing only with the Xadow board
#define cs                  (A5)
#define dc                  (3)
#define mosi                (16)
#define sclk                (15)

#define LEFT_GUTTER_SIZE    (FONT_X)
#define FONT_SIZE           (1)
#define LINE_HEIGHT         (FONT_SIZE*FONT_Y+4)

#define SLEEP_TIME_MS       (250)

SSD1331 oled = SSD1331(cs, dc, mosi, sclk);

char *test_menu[] = {"hello", "world", "here be", "dragons", NULL};

char _sbuf[256];
char _sbuf2[64];

uint16_t now = 0;
uint16_t checkpoint;

typedef enum {
  SCREEN_WATCH,
  SCREEN_STATUS,
  SCREEN_OTHER,
} Screen;

Screen currentScreen;

void setup() {
  pinMode(10, INPUT);

  Xadow.init();

  oled.init();
  oled.fillScreen(COLOR_BLACK);
  animation_boot();
  oled.fillScreen(COLOR_BLACK);

  menu_init();
  status_init();
  watch_init(now);
  batlog_init(0x33);
  sync_init();

  oled.fillScreen(COLOR_BLACK);

  currentScreen = SCREEN_OTHER;

  oled.drawString("Disconnect USB", 0, 0, FONT_SIZE, COLOR_RED);
  oled.drawString("then press WAKE", 0, LINE_HEIGHT, FONT_SIZE, COLOR_RED);
  while(digitalRead(10) == HIGH);;

  mcu_reduce_features();

  now = 0;
  checkpoint = millis();
}

void loop() {
  power_spi_disable();

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

  power_spi_enable();
  uint8_t changes = watch_tick(now);

  batlog_tick(changes);

  Screen nextscreen = currentScreen;

  if (digitalRead(10) == LOW) {
    nextscreen = SCREEN_STATUS;
  } else {
    nextscreen = SCREEN_WATCH;
  }

  if (nextscreen != currentScreen) {
    oled.fillScreen(COLOR_BLACK);
    currentScreen = nextscreen;
  }

  switch(nextscreen) {
    case SCREEN_STATUS:
      status_show();
      delay(3000);
      break;

    case SCREEN_WATCH:
      watch_show(changes);
      break;
  }


  uint16_t t = millis();
  now += t - checkpoint;
  checkpoint = t;
}
