CC=g++
CFLAGS= -Wall -g -std=c++17

all: main

main: main.cpp HttpAPI.o TCPServer.o HttpServer.o HttpRequest.o HttpResponse.o Utils.o
	$(CC) $(CFLAGS) -o $@ $^

HttpAPI.o: HttpAPI.cpp HttpAPI.h
	$(CC) $(CFLAGS) -c $<

TCPServer.o: ../Networking/TCPServer.cpp ../Networking/TCPServer.h
	$(CC) $(CFLAGS) -c $<

HttpServer.o: ../Networking/HttpServer.cpp ../Networking/HttpServer.h
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