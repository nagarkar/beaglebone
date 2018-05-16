#!/usr/bin/python
# Usage:  python gpsd_client.py localhost 2947

import serial
import sys
import gps

host = sys.argv[1]
port = sys.argv[2]

session = gps.gps(host, port)
session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)
while True:
	report = session.next()
	if (report['class'] == 'TPV'):
		# Print the attributes. 
		# For full list, see http://www.catb.org/gpsd/gpsd_json.html
		sys.stdout.write("TIME: 	%s " %report.time)
		sys.stdout.write("GPS Status: 	%d " %report.status)
		sys.stdout.write("Latitude: 	%d " %report.lat)
		sys.stdout.write("Longitude: 	%d " %report.lon)
		sys.stdout.write("Speed: 	%d " %report.speed)
		sys.stdout.write("Course: 	%d " %report.track)
		

		# We want to print in place on console
		# See https://goo.gl/bhDYz2 for explanation of this trick.
		sys.stdout.write("\r")
		sys.stdout.flush()

