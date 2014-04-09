# Makefile for exercise 2
# 2003-11-26

#LDFLAGS = -L. -L/home/mbscholt/mydocs/vu/20032004/cg/exercises -L/usr/include -lglut -L/usr/lib
CFLAGS=-I../ -I/usr/include
LDFLAGS=-lm -lGLU -lGL -lglut -L/usr/lib

#LDFLAGS = -L. -L../ -L/usr/include -lglut -L/usr/lib -L../util -I../ -I/home/mbscholt/mydocs/vu/20032004/cg/exercises -L/home/mbscholt/mydocs/vu/20032004/cg/exercises

all:	ex2

ex2:	main.o ../util/readtex.c
	gcc $(CFLAGS) -Wall -o main ../util/readtex.c main.o $(LDFLAGS)

#ex2:	main.o ../util/readtex.c
#	gcc -Wall -o main ../util/readtex.c main.o $(LDFLAGS)

#main: main.c ../util/glutil.c ../util/sgf_loader.o
#	$(CC) $(CFLAGS) main.c ../util/glutil.c ../util/sgf_loader.o $(LDFLAGS) -o exercise2

clean:
	rm -rf main.o main core
