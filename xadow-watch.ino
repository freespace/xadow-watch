#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include "SGL.h"
#include "SSD1331.h"

#include "xadow.h"

// These are arduino pin definitions. Note that on leonardo
// boards MOSI and sclk cannot be directly used, and hence
// D16 and D15 is a thing only with the Xadow board
#define cs       (A5)
#define dc       (3)
#define mosi     (16)
#define sclk     (15)

#define LEFT_GUTTER_SIZE    (FONT_X)
#define FONT_SIZE           (1)
#define LINE_SPACING        (FONT_SIZE*FONT_Y+4)

SSD1331 oled = SSD1331(cs, dc, mosi, sclk);  

char *test_menu[] = {"hello", "world", "here be", "dragons", NULL};

char _sbuf[64];
char _sbuf2[64];

uint8_t logbat = 0;
uint16_t eeaddr = 1;

uint16_t now = 0;
uint16_t checkpoint;


typedef enum {
  SCREEN_WATCH,
  SCREEN_STATUS,
} Screen;

Screen currentScreen;

void setup() {
  pinMode(10, INPUT);

  Xadow.init();
  //Xadow.greenLed(LEDON);

  oled.init();
  oled.fillScreen(COLOR_BLACK);
  oled.drawString("BOOTING", (RGB_OLED_WIDTH-FONT_X*6)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(1000);
  oled.fillScreen(COLOR_BLACK);
  
  menu_init();
  status_init();
  watch_init(now);
  
  status_show();
  
  delay(1000);
  
  menu_show(test_menu);
  
  delay(1000);
  
  oled.fillScreen(COLOR_BLACK);
  watch_show(0xFF);  

  if (EEPROM[0] != 0x05) {
    EEPROM[0] = 0x05;
    logbat = 1;
  } else {
    Serial.begin(9600);
    Serial.println("Battery log detected, not overwriting");
    for (eeaddr = 1; eeaddr < 1024; ++eeaddr) {
      Serial.println(EEPROM[eeaddr], DEC);
    }
  }

  power_twi_disable();
  power_usart1_disable();
  power_usb_disable();
  power_adc_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_timer3_disable();

  now = 0;
  checkpoint = millis();
  currentScreen = SCREEN_WATCH;
}

void loop() {
  power_spi_disable();

  // sleep for a bit to save power. This is OK b/c timer0 which provides
  // millis is not disabled
  Xadow.goToSleep(SLEEP_MODE_PWR_DOWN, 200);

  power_spi_enable();
  uint8_t changes = watch_tick(now);

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
      delay(1000);
      break;

    case SCREEN_WATCH:
      watch_show(changes);
      break; 
  }

  if (changes >= 2 && logbat) {
    if (eeaddr < 1024) {
      uint8_t batvol = Xadow.getBatVol()*10;
      EEPROM[eeaddr] = batvol;
      eeaddr += 1;
    }
  }

  uint16_t t = millis();
  now += millis() - checkpoint + 200;
  checkpoint = t;
}
