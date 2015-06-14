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

  // This is a milli-second correction that is applied every 1000
  // seconds, which means it is a part-per-million correction.
  //
  // This adjustment increases or decreases the time to the next second when
  // it is applied, and has affect for 1 second. If it is say, 1000, then
  // when it is applied, the watch will take another 1000 ms before
  // advancing the seconds counter.
  //
  // If the watch is running fast, then we want to make this positive to make
  // the watch take longer to increment a second.
  //
  // If the watch is running slow, then we want to make this negative.
  //
  // If the watch is exactly on time, then it shuold be 0.
  millis_delta_t millis_adjust;

  // number of seconds since we last applied millis_adjust.
  uint16_t seconds_since_last_adjust;

  uint16_t color;

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
  Watch.color = WATCH_COLOR;
}

/**
This function will draw the watch face. Be sure to call watch_tick()
to update time keeping!

See README.md for how changes is interpreted.

If 0 is passed in, nothing happens.
*/
void watch_show(uint8_t changes) {
  if (changes == 0) return;

#define W     Watch
  uint8_t xpos = 0, ypos = 0;

  uint8_t toclear;

  W.needs_redraw |= changes & REDRAW_FLAG;
  changes &= ~REDRAW_FLAG;

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

  uint8_t clearwidth = SCREEN_WIDTH*toclear/8;
  xpos = SCREEN_WIDTH - clearwidth;
  oled.clearWindow(xpos, ypos, xpos+clearwidth, ypos + FONT_Y*2);

  sprintf(_sbuf, "%02d:%02d:%02d", W.hours, W.minutes, W.seconds);
  oled.drawString(_sbuf, 0, ypos, FONT_SIZE*2, W.color);

  if (changes > 3 || W.needs_redraw) {
    ypos += LINE_HEIGHT*2;

    oled.clearWindow(0, ypos, SCREEN_WIDTH, ypos+FONT_Y);

    sprintf(_sbuf, "%04d/%02d/%02d", W.year, W.month, W.day);
    xpos = SCREEN_WIDTH - (8*FONT_X);
    xpos /= 2;
    oled.drawString(_sbuf, xpos, ypos, FONT_SIZE, W.color);
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
    W.seconds_since_last_adjust += 1;

    // apply millis_adjust every 1,000,000 milliseconds
    if (W.seconds_since_last_adjust >= 1000) {
      W.next_second_millis += W.millis_adjust;
      W.seconds_since_last_adjust = 0;

      if (W.color == COLOR_BLUE) W.color = COLOR_RED;
      else W.color = COLOR_BLUE;
    }

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

    // TODO: implement handling for leap year
    // TODO: implement handling for year changes
    if (W.month >= 12) {
      W.month = 0;
      changes += 1;
    }

    return changes;
  } else return 0;
#undef W
}

void watch_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds, millis_delta_t millis_adjust) {
  Watch.seconds = seconds;
  Watch.minutes = minutes;
  Watch.hours = hours;
  Watch.millis_adjust = millis_adjust;
  Watch.seconds_since_last_adjust = 0;
  Watch.needs_redraw = 1;
}

void watch_set_date(uint16_t year, uint8_t month, uint8_t day, uint8_t leapyear) {
  Watch.year = year;
  Watch.month = month;
  Watch.day = day;
  Watch.leapyear = leapyear;
  Watch.needs_redraw = 1;
}
