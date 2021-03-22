all: main.out libhook.so

main.out: main.c
	gcc main.c -o main.out

libhook.so: hook.c
	gcc -fPIC -DPIC hook.c -shared -o ./libhook.so -std=c99 -ldl
