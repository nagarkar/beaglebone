v4l2grab: v4l2grab.c
	gcc -O2 -Wall `pkg-config --cflags --libs libv4l2` v4l2grab.c -o v4l2grab

all:v4l2grab

clean:
	rm -rf *.o
