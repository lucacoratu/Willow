CC=g++
CFLAGS= -Wall -g -std=c++17

all: main

main: main.cpp TCPServer.o HttpServer.o HttpResponse.o HttpRequest.o Utils.o
	$(CC) $(CFLAGS) $^ -o $@ 

TCPServer.o: TCPServer.cpp TCPServer.h
	$(CC) $(CFLAGS) -c $<

HttpServer.o: HttpServer.cpp HttpServer.h
	$(CC) $(CFLAGS) -c $< 

HttpResponse.o: ../Parser/HttpResponse.cpp ../Parser/HttpResponse.h
	$(CC) $(CFLAGS) -c $<

HttpRequest.o: ../Parser/HttpRequest.cpp ../Parser/HttpRequest.h
	$(CC) $(CFLAGS) -c $<

Utils.o: ../Parser/Utils.cpp ../Parser/Utils.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o main