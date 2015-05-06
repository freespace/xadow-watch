void animation_boot () {
  oled.drawString("[    ]", (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(200);
  oled.drawString("[|   ]", (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(200);
  oled.drawString("[||  ]", (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(200);
  oled.drawString("[||| ]", (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(200);
  oled.drawString("[||||]", (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(200);
  oled.drawString("[||||]", (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLACK);
  oled.drawString("READY!", (RGB_OLED_WIDTH-FONT_X*5)/2, RGB_OLED_HEIGHT/2-FONT_Y/2, 1, COLOR_BLUE);
  delay(1000);
}
