# Makefile for exercise 2
# 2003-11-18

#LDFLAGS = -L. -L/home/mbscholt/mydocs/vu/20032004/cg/exercises -L/usr/include -lglut -L/usr/lib
LDFLAGS = -L. -L../ -L/usr/include -lglut -L/usr/lib -L../util -I../ -I/home/mbscholt/mydocs/vu/20032004/cg/exercises -L/home/mbscholt/mydocs/vu/20032004/cg/exercises

all:	ex2

ex2:	main.o
	gcc -Wall -o main main.o $(LDFLAGS)

#main: main.c ../util/glutil.c ../util/sgf_loader.o
#	$(CC) $(CFLAGS) main.c ../util/glutil.c ../util/sgf_loader.o $(LDFLAGS) -o exercise2

clean:
	rm -rf main.o main core
