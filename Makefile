# Makefile
all: libmylib.a main

libmylib.a: mylib.o
	ar rcs libmylib.a mylib.o

mylib.o: mylib.c mylib.h
	gcc -c mylib.c

main: main.c libmylib.a
	gcc main.c -L. -lmylib -o main

run_client: main
	./main

server: server.c libmylib.a
	gcc server.c -L. -lmylib -o server

run_server: server
	./server

clean:
	rm -f libmylib.a mylib.o main

