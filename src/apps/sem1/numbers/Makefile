all: app

app: main.o sample.o
	gcc main.o sample.o -o app

main.o: main.c sample.h
	gcc main.c -c

sample.o: sample.cpp
	g++-10 sample.cpp -c

clean:
	rm sample.o main.o
