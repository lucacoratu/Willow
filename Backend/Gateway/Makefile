CC=g++
CFLAGS= -Wall -g -std=c++17
LFLAGS= -lHttpServer -L../HttpServer/

all: main

main: main.cpp libHttpServer.a
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

libHttpServer.a: 
	cd ../HttpServer; make all