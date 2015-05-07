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

  for (uint8_t idx = 0; sequence[idx] != NULL; ++idx) {
    if (sequence[idx+1] == NULL) {
      oled.drawString(sequence[idx-1], (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLACK);
    }

    oled.drawString(sequence[idx], (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
    delay(500);
  }
  delay(1000);
}
