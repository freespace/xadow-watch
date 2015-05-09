#define SYNC_TEXT_COLOR     (COLOR_WHITE)

#define SYNC_DELIMITER      ';'

void sync_init() {
}

void sync_abort(char *msg) {
  Serial.print("SYNC aborted:");
  Serial.println(msg);
}

/**
 * Checks serial connection for start of sync.
 *
 * Sync protocol uses ; as delimiter, and is as follows:
 *
 * The characters SYNC is sent, followed by ;
 *
 *    SYNC;
 *
 * Then 3 integers separated by ; are sent representing the current hour,
 * minute, and second
 *
 *    5;32;22;
 *
 * Then 4 integers, separated by ; are sent representing the current year,
 * month, and day of month, and whether the current year is a leap year
 *
 *    2015;05;03;0;
 *
 * Termination of sync is denoted by END
 *
 *    END;
 *
 * White spaces may be inserted before/after ; and will be ignored. E.g.
 *
 *    2015;    05;   03;0
 *
 * is valid.
 */ 
void sync_listen() {
  uint8_t readpos = 0;
  for (readpos = 0; Serial.available() && readpos < sizeof(_sbuf)-1; ++readpos) {
    _sbuf[readpos] = Serial.read();
  }

  if (readpos == 0) return;
  
  _sbuf[readpos] = '\0';

  if (strstr(_sbuf, "SYNC;") == _sbuf) {
    // we have start of sync, yay
    char *sptr = _sbuf+5;
    Serial.println("Starting SYNC");
  } else sync_abort("SYNC; expected");
}
