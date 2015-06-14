void animation_boot () {
  char *sequence [] = {
    "[    ]",
    "[|   ]",
    "[||  ]",
    "[||| ]",
    "[||||]",
    "[DONE]",
    NULL
  };

  oled.setBufferRegion(0, 16, 128, 32);
  uint8_t w = 128, h=32;

  for (uint8_t idx = 0; sequence[idx] != NULL; ++idx) {
    if (sequence[idx+1] == NULL) {
      oled.drawString(sequence[idx-1], (w*5)/2, h/2-FONT_Y/2, 1, COLOR_BLACK);
      oled.uploadBuffer();
    }

    oled.clearBuffer();
    oled.drawString(sequence[idx], (w-FONT_X*5)/2, h/2-FONT_Y/2, 1, COLOR_BLUE);
    oled.uploadBuffer();
    delay(200);
  }
  delay(1000);
}
