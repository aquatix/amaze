#LDFLAGS = -L. -L/home/mbscholt/mydocs/vu/20032004/cg/exercises -L/usr/include -lglut -L/usr/lib
LDFLAGS = -L. -L.. -L/usr/include -lglut -L/usr/lib -L../util -I.. -I/home/mbscholt/mydocs/vu/20032004/cg/exercises -L/home/mbscholt/mydocs/vu/20032004/cg/exercises

#ex1:	main.o
#	f77 -o ex1  main.o
ex1:	main.o
	gcc -Wall -o main main.o $(LDFLAGS)

clean:
	rm -rf main.o main core
