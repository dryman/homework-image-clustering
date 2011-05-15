all:	*.c
	cc `pkg-config --cflags --libs opencv` *.c -O3 -std=c99 -Wall -o cluster
debug:	*.c
	cc `pkg-config --cflags --libs opencv` *.c -g -std=c99 -Wall
