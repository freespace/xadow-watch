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

void setup() {
  Xadow.init();
  //Xadow.greenLed(LEDON);

  oled.init();
  oled.fillScreen(COLOR_BLACK);
  oled.drawString("BOOTING", (RGB_OLED_WIDTH-FONT_X*6)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(1000);
  
  menu_init();
  status_init();
  watch_init();
  
  status_show();
  
  delay(3000);
  
  menu_show(test_menu);
  
  delay(3000);
  
  oled.fillScreen(COLOR_BLACK);
  watch_show(0xFF);  

  Serial.begin(9600);
  if (EEPROM[0] != 0x03) {
    EEPROM[0] = 0x03;
    logbat = 1;

    Serial.println("Battery log started");
  } else {
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
}

void loop() {
  power_spi_disable();

  // sleep for a bit to save power. This is OK b/c timer0 which provides
  // millis is not disabled
  Xadow.goToSleep(SLEEP_MODE_IDLE, 100);

  power_spi_enable();
  uint8_t changes = watch_tick(millis());
  if (changes) {
    watch_show(changes);
    //status_show();
    // log voltage every minute
    if (changes >= 2 && logbat) {
      if (eeaddr < 1024) {
        uint8_t batvol = Xadow.getBatVol()*10;
        EEPROM[eeaddr] = batvol;
        eeaddr += 1;
      }
    }
  }
}
