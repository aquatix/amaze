# Makefile for Project:aMaze
# 2004-01-03

CFLAGS=-I../ -I/usr/include
LDFLAGS=-lm -lGLU -lGL -lglut -L/usr/lib

all:	amaze

amaze:	main.o ../util/readtex.c
	#gcc $(CFLAGS) -Wall -o main ../util/readtex.c main.o $(LDFLAGS)
	gcc $(CFLAGS) -Wall -gnatp -o main ../util/readtex.c main.o $(LDFLAGS)

clean:
	rm -rf main.o main core
