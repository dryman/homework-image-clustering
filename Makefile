all:	*.c
	cc `pkg-config --cflags --libs opencv` *.c -O3 -std=c99 -Wall
