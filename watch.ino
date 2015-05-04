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
  _watch_state.next_second_millis = millis() + 1000;
}

/**
This function will draw the watch face. Be sure to call watch_tick()
to update time keeping!

To redraw the entire watch face, pass in changes = 0xff. Otherwise pass in
whatever is returned by watch_tick() for optimised update of watch face.
*/
void watch_show(uint8_t changes) { 
  uint8_t seconds = _watch_state.seconds;
  uint8_t minutes = _watch_state.minutes;
  uint8_t hours = _watch_state.hours;
  uint8_t day = _watch_state.day;
  uint8_t month = _watch_state.month;  
  
  uint8_t xpos = 0, ypos = 0;
  
  uint8_t toclear;
  switch(changes) {
     case 1:
       toclear = 2;
       break;
     case 2:
       toclear = 5;
       break;
     case 0xFF:
     case 3:
     default:
       toclear = 8;
       break;
  }
  
  uint8_t clearwidth = RGB_OLED_WIDTH*toclear/8;
  oled.clearWindow(RGB_OLED_WIDTH-clearwidth, ypos, RGB_OLED_WIDTH, FONT_Y*2);
  
  sprintf(_sbuf, "%02d:%02d:%02d", hours, minutes, seconds);
  oled.drawString(_sbuf, xpos, ypos, FONT_SIZE*2, WATCH_COLOR);  
}

/**
This function performs time keeping. To draw the watch face call
watch_show().

This function returns an integer indiciating the number of changed counters:

0 - no change
1 - seconds
2 - minutes
3 - hours
4 - day
5 - month

Because all changes start with seconds and propagate forward, a change in a high
value counter implies all counter below it have also changed. e.g. if 2 is returned,
then it is implied that seconds and minutes have also changed.

Note that this kind of time keeping on the Xadow has an error of about 1000 ppm, which
is around 30 seconds over 8 hours.
*/
uint8_t watch_tick () {
  int16_t elapsed = millis() - _watch_state.next_second_millis;
  if (elapsed > 0) {
    _watch_state.next_second_millis += 1000;
    
    uint8_t seconds = _watch_state.seconds;
    uint8_t minutes = _watch_state.minutes;
    uint8_t hours = _watch_state.hours;
    uint8_t day = _watch_state.day;
    uint8_t month = _watch_state.month;
  
    uint8_t changes = 1;
    
    seconds += 1;
    if (seconds >= 60) {
      seconds = 0;
      minutes += 1;
      changes += 1;
    }
    
    if (minutes >= 60) {
      minutes = 0;
      hours += 1;
      changes += 1;
    }
    
    if (hours >= 24) {
      hours = 0;
      day += 1;
      changes += 1;        
    }
    
    if (day >= _watch_days_per_month[month]) {
      day = 0;
      month += 1;
      changes += 1;        
    }
    
    if (month >= 12) {
      month = 0;
      changes += 1;
    }
  
    _watch_state.seconds = seconds;
    _watch_state.minutes = minutes;  
    _watch_state.hours = hours;
    _watch_state.day = day;
    _watch_state.month = month;
    
    return changes;
  } else return 0;
}
