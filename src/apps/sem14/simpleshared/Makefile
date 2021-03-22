all: main.out

main.out: main.c libtest.so
	gcc main.c -o main.out -L./ -I./test -ltest

libtest.so: test/test.c test/test.h
	gcc -fPIC -DPIC test/test.c -shared -o ./libtest.so -std=c99
