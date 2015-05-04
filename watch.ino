#define WATCH_COLOR        (COLOR_BLUE)

typedef struct {
  uint16_t next_second_millis;
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t month;
  uint8_t day;
} WatchState;

uint8_t _watch_days_per_month[] = {
  31, // jan
  28, // feb, ignoring leap years for now
  31, // mar
  30, // apr
  31, // may
  30, // jun
  31, // jul
  31, // aug
  30, // sep
  31, // oct
  30, // nov
  31, // dec
};

WatchState _watch_state = {0};

void watch_init() {
  watch_update();
}

void _watch_tick () {
  uint8_t seconds = _watch_state.seconds;
  uint8_t minutes = _watch_state.minutes;
  uint8_t hours = _watch_state.hours;
  uint8_t day = _watch_state.day;
  uint8_t month = _watch_state.month;

  seconds += 1;
  if (seconds >= 60) {
    seconds = 0;
    minutes += 1;
  }
  
  if (minutes >= 60) {
    minutes = 0;
    hours += 1;
  }
  
  if (hours >= 24) {
    hours = 0;
    day += 1;
  }
  
  if (day >= _watch_days_per_month[month]) {
    day = 0;
    month += 1;
  }
  
  if (month >= 12) month = 0;
  
  uint8_t xpos = 0, ypos = 0;
  
  uint8_t toclear = 2;
  if (minutes != _watch_state.minutes) toclear += 3;
  if (hours != _watch_state.hours) toclear += 3;
  
  uint8_t clearwidth = RGB_OLED_WIDTH*toclear/8;
  oled.clearWindow(RGB_OLED_WIDTH-clearwidth, ypos, RGB_OLED_WIDTH, FONT_Y*2);
  
  sprintf(_sbuf, "%02d:%02d:%02d", hours, minutes, seconds);
  oled.drawString(_sbuf, xpos, ypos, FONT_SIZE*2, WATCH_COLOR);

  _watch_state.seconds = seconds;
  _watch_state.minutes = minutes;  
  _watch_state.hours = hours;
  _watch_state.day = day;
  _watch_state.month = month;
}

/**
Updates when the next seconds is due without modifying anything. Call this prior to
entering loop
*/

void watch_update () {
  _watch_state.next_second_millis = millis()+1000;
}

void watch_show() {
  int16_t elapsed = millis() - _watch_state.next_second_millis;
  if (elapsed > 0) {
      // we have past when we should have incremented by a second
      _watch_tick();
    
      _watch_state.next_second_millis += 1000;
    }
}
