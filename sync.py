import serial
import sys
ser = serial.Serial(sys.argv[1], 9600);

from datetime import datetime
from calendar import isleap

now = datetime.now()

leap = 0
if isleap(now.year):
  leap = 1

parts = [
  'SYNC',
  now.hour,
  now.minute,
  now.second,
  now.year,
  now.month,
  now.day,
  leap,
  'END'
];
s = ';'.join(map(str, parts))
print '>>',s
ser.write(';'.join(map(str, parts)))
print ser.readline()
print ser.readline()
ser.close()

