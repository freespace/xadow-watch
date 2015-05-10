#define WATCH_COLOR        (COLOR_BLUE)

typedef struct {
  millis_t next_second_millis;
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t month;
  uint8_t day;

  uint16_t year;
  // set to 1 if year is a leap year, 0 otherwise
  uint8_t leapyear;

  // set this when we need to redraw everything regardless of what
  // watch_tick returns.
  uint8_t needs_redraw;
} Watch_t;

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

Watch_t Watch = {0};

void watch_init(millis_t now) {
  Watch.next_second_millis = now;
  Watch.needs_redraw = 1;
}

/**
This function will draw the watch face. Be sure to call watch_tick()
to update time keeping!

To redraw the entire watch face, pass in changes = 0xff. Otherwise pass in
whatever is returned by watch_tick() for optimised update of watch face.

If 0 is passed in, nothing happens.
*/
void watch_show(uint8_t changes) {
  if (changes == 0) return;

#define W     Watch
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

  if (W.needs_redraw) toclear = 8;

  uint8_t clearwidth = RGB_OLED_WIDTH*toclear/8;
  xpos = RGB_OLED_WIDTH - clearwidth;
  oled.clearWindow(xpos, ypos, xpos+clearwidth, ypos + FONT_Y*2);

  sprintf(_sbuf, "%02d:%02d:%02d", W.hours, W.minutes, W.seconds);
  oled.drawString(_sbuf, 0, ypos, FONT_SIZE*2, WATCH_COLOR);

  if (changes > 3 || W.needs_redraw) {
    ypos += LINE_HEIGHT*2;

    oled.clearWindow(0, ypos, RGB_OLED_WIDTH, ypos+FONT_Y);

    sprintf(_sbuf, "%04d/%02d/%02d", W.year, W.month, W.day);
    xpos = RGB_OLED_WIDTH - (8*FONT_X);
    xpos /= 2;
    oled.drawString(_sbuf, xpos, ypos, FONT_SIZE, WATCH_COLOR);
  }

  W.needs_redraw = 0;
#undef W
}

/**
This function performs time keeping. To draw the watch face call
watch_show().

  now:  the number of milliseconds since some epoch which uniquely identifies the
        current time

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
uint8_t watch_tick (millis_t now) {
#define W     Watch
  millis_delta_t elapsed = now - W.next_second_millis;

  if (elapsed > 0) {
    W.next_second_millis += 1000;

    uint8_t changes = 1;

    W.seconds += 1;
    if (W.seconds >= 60) {
      W.seconds = 0;
      W.minutes += 1;
      changes += 1;
    }

    if (W.minutes >= 60) {
      W.minutes = 0;
      W.hours += 1;
      changes += 1;
    }

    if (W.hours >= 24) {
      W.hours = 0;
      W.day += 1;
      changes += 1;
    }

    if (W.day >= _watch_days_per_month[W.month]) {
      W.day = 0;
      W.month += 1;
      changes += 1;
    }

    if (W.month >= 12) {
      W.month = 0;
      changes += 1;
    }

    return changes;
  } else return 0;
#undef W
}

void watch_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  Watch.seconds = seconds;
  Watch.minutes = minutes;
  Watch.hours = hours;
  Watch.needs_redraw = 1;
}

void watch_set_date(uint16_t year, uint8_t month, uint8_t day, uint8_t leapyear) {
  Watch.year = year;
  Watch.month = month;
  Watch.day = day;
  Watch.leapyear = leapyear;
  Watch.needs_redraw = 1;
}
