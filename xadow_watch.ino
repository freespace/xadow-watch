#include <Wire.h>
#include <SPI.h>

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

void setup() {
  Xadow.init();
  Xadow.greenLed(LEDON);

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
  watch_update();
}

void loop() {
  watch_show();
}
