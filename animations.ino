void animations_boot () {
  char *sequence [] = {
    "[    ]",
    "[|   ]",
    "[||  ]",
    "[||| ]",
    "[||||]",
    "[DONE]",
    NULL
  };

  oled.setBufferRegion(SSD1306_REGION_MID);
  uint8_t w = 128, h=32;

  for (uint8_t idx = 0; sequence[idx] != NULL; ++idx) {
    if (sequence[idx+1] == NULL) {
      oled.drawString(sequence[idx-1], (w-FONT_SPACE*6)/2, h/2-FONT_Y/2, 1, COLOR_BLACK);
    }

    oled.clearBuffer();
    oled.drawString(sequence[idx], (w-FONT_SPACE*6)/2, h/2-FONT_Y/2, 1, COLOR_BLUE);
    delay(200);
  }
  delay(1000);
}
