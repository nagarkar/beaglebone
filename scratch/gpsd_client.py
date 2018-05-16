#!/usr/bin/python
# Usage:  python gpsd_client.py localhost 2947

import serial
import sys
import gps
import curses

host = sys.argv[1]
port = sys.argv[2]

# Helper method to work around the fact python doesnt' support i++ syntax
def _i():
  _i.count += 1
  return _i.count;

# Main method
def run(stdscr):
  session = gps.gps(host, port)
  session.stream(gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)

  stdscr.clear()
  done = False
  while not done:
	report = session.next()
	
	_i.count = 0
	stdscr.addstr(_i(),0, "Type x to exit")
	if (report['class'] == 'TPV'):
		# Print the attributes. 
		# For full list, see http://www.catb.org/gpsd/gpsd_json.html
		stdscr.addstr(_i(),0, "Time: 	%s" %report.time)
		stdscr.addstr(_i(),0, "Device:%s" %report.device)
		stdscr.addstr(_i(),0, "Lat: 	%s" %report.lat)
		stdscr.addstr(_i(),0, "Long: 	%s" %report.lon)
		stdscr.addstr(_i(),0, "Speed: 	%s" %report.speed)
		stdscr.addstr(_i(),0, "Track: 	%s" %report.track)
		stdscr.refresh();
	stdscr.nodelay(True)
	key = stdscr.getch()
	if key == ord('x'):
		done = True
curses.wrapper(run)
