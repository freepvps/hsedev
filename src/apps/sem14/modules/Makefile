all: main.out libmodule.so

main.out: main.c
	gcc main.c -o main.out -ldl

libmodule.so: module.c
	gcc -fPIC -DPIC module.c -shared -o ./libmodule.so -std=c99 -nostartfiles
