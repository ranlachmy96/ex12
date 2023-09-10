CC = gcc
CFLAGS = -std=c11

all: main.o http_protocol.o httpd.o router.o
    $(CC) $(CFLAGS) -o httpd main.o http_protocol.o httpd.o router.o

http_protocol.o: http_protocol.c httpd.h
    $(CC) $(CFLAGS) -c http_protocol.c -o http_protocol.o

httpd.o: http_server.c httpd.h
    $(CC) $(CFLAGS) -c http_server.c -o httpd.o

router.o: router.c httpd.h
    $(CC) $(CFLAGS) -c router.c -o router.o

clean:
    rm -f httpd main.o http_protocol.o httpd.o router.o
