#define SYNC_TEXT_COLOR     (COLOR_WHITE)

#define SYNC_DELIMITER      ';'

// timeout in milliseconds. If we expect communication and don't see it for
// this long we abort sync
#define SYNC_SERIAL_TIMEOUT (1*3000)

typedef struct {
  uint8_t abort;
} Sync_t;

Sync_t Sync = {0};

void sync_init() {
}

void sync_abort(char *msg) {
  Serial.print("SYNC aborted:");
  Serial.println(msg);
}

int8_t sync_blocking_read() {
  int8_t c;
  uint16_t startt = millis();

  do {
    c = Serial.read();
    if (millis() - startt > SYNC_SERIAL_TIMEOUT) {
      sync_abort("communication timed out");
      Sync.abort = 1;
      break;
    }
  } while (c == -1);
  return c;
}

/**
 * Looks for the given word in the serial input.
 */ 
uint8_t sync_get_word(char *w) {
  char *p = w;
  uint8_t c;
  while(*p && Sync.abort == 0) {
    c = sync_blocking_read();
    if (c == *p) p++;
    else return ERROR;
  }

  return OK;
}

/**
 * Reads serial stream until SYNC_DELIMITER is found or out of space, then
 * uses atoi to convert the string into an integer, returning the result.
 */
int16_t sync_get_int() {
  uint8_t readpos = 0;
  uint8_t c;
  do {
    c = sync_blocking_read();
    if (c == SYNC_DELIMITER) break;
    else _sbuf[readpos++] = c;
  } while (readpos < sizeof(_sbuf)-1 && Sync.abort == 0);

  _sbuf[readpos] = '\0';
  return atoi(_sbuf);
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

#define CHECK_ABORT()     do { if (Sync.abort) return; } while (0);

void sync_listen() {
  Sync.abort = 0;
  if (Serial.available()) {
    if (sync_get_word("SYNC;") == OK) {
      Serial.println("SYNC START");
    } else {
      sync_abort("SYNC; expected");
      return;
    }

    uint8_t hours = sync_get_int();
    uint8_t minutes = sync_get_int();
    uint8_t seconds = sync_get_int();

    CHECK_ABORT();

    sprintf(_sbuf, "Time: %02d:%02d:%02d\n", hours, minutes, seconds);
    Serial.print(_sbuf);
  } 
}
