CFLAGS=-c `pkg-config --cflags gtk+-2.0` -Wall
LDFLAGS=`pkg-config --libs gtk+-2.0` -lportaudio -lm

all:
	gcc $(CFLAGS) pendulum.c
	gcc -o pendulum pendulum.o $(LDFLAGS)
