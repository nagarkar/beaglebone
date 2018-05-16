#!/usr/bin/python
# Usage: $ python gpsloc.py

import serial
import sys

device = "/dev/ttyUSB0"
if (len(sys.argv) > 1):
  device = sys.argv[1]

ser = serial.Serial(device, 4800, timeout = 1)
raw_data = ser.read(500)

# Get first record in raw data
start = raw_data.find("$GPRMC")
end = raw_data.find("\n", raw_data.find("$GPRMC"))
gps_record = raw_data[start:end].split(',')

# Print record
print "Lattitude: " + gps_record[3] + gps_record[4]
print "Longitude: " + gps_record[5] + gps_record[6]
print "Speed: " + gps_record[7]
print "Course: " + gps_record[8]
