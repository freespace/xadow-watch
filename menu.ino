#define MENU_COLOR_DEFAULT  (COLOR_BLUE)
#define MENU_COLOR_SELECTED (COLOR_CYAN)

typedef void (*menu_item_selected)(uint8_t selected);

typedef struct {
  char **current_items;
  uint8_t selected_item_index;
} MenuState;

MenuState _menu_state = {NULL, 0};

void menu_init() {
}

/**
Items is expected to be an array of char *, each pointing
to a null terminated string. The array itself is expected to
be terminated with a NULL pointer.

Menu items will be displayed in the order given, with scrolling
indicators added if required.

When an item is selected, the callback function, set via
menu_set_callback, will be called with the 0 based index of the
selected string as argument.
*/
void menu_show(char *items[]) {
  uint8_t idx;
  uint8_t xpos = LEFT_GUTTER_SIZE, ypos = 0;
  uint16_t item_color;
  
  _menu_state.current_items = items;  
  oled.fillScreen(COLOR_BLACK);
  
  for (idx = 0; items[idx]; ++idx) {
    item_color = MENU_COLOR_DEFAULT;
    if (idx == _menu_state.selected_item_index) {
      item_color = MENU_COLOR_SELECTED;      
      oled.drawChar('>', 0, ypos, FONT_SIZE, item_color);
    }
      
    oled.drawString(items[idx], xpos, ypos, FONT_SIZE, item_color);
    
    ypos += LINE_SPACING;
  }
}

