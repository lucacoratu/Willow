CC=g++
CFLAGS= -Wall -std=c++17

all: libHttpServer.a

libHttpServer.a: HttpAPI.o TCPServer.o HttpServer.o HttpClient.o HttpRequest.o HttpResponse.o Utils.o
	ar rc $@ $^

HttpAPI.o: src/API/HttpAPI.cpp src/API/HttpAPI.h
	$(CC) $(CFLAGS) -c $<

TCPServer.o: src/Networking/TCPServer.cpp src/Networking/TCPServer.h
	$(CC) $(CFLAGS) -c $<

HttpServer.o: src/Networking/HttpServer.cpp src/Networking/HttpServer.h
	$(CC) $(CFLAGS) -c $<

HttpClient.o: src/Networking/HttpClient.cpp src/Networking/HttpClient.h
	$(CC) $(CFLAGS) -c $<

HttpResponse.o: src/Parser/HttpResponse.cpp src/Parser/HttpResponse.h
	$(CC) $(CFLAGS) -c $<

HttpRequest.o: src/Parser/HttpRequest.cpp src/Parser/HttpRequest.h
	$(CC) $(CFLAGS) -c $<

Utils.o: src/Parser/Utils.cpp src/Parser/Utils.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o libHttpServer.a