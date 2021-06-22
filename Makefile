IDIR=lib
CC=gcc
CFLAGS=-g -Wall -I $(IDIR) -lm

ODIR=out
SDIR=src

locations.o: lib/locations.h
	mkdir -p out && gcc -c -o out/locations.o src/locations.c -g -Wall -I lib -lm

utils.o: lib/utils.h
	mkdir -p out && gcc -c -o out/utils.o src/utils.c -g -Wall -I lib -lm

server.o: 
	mkdir -p out && gcc -c -o out/server.o src/server.c -g -Wall -I lib -lm

client.o:
	mkdir -p out && gcc -c -o out/client.o src/client.c -g -Wall -I lib -lm

server: locations.o utils.o server.o
	gcc -o server out/locations.o out/utils.o out/server.o -g -Wall -I lib -lm

client: utils.o client.o
	gcc -o client out/utils.o out/client.o -g -Wall -I lib -lm

all: server client

.PHONY: clean

clean:
		rm -f $(ODIR)/*.o
		rm -f $(TARGET_CLIENT) $(TARGET_SERVER)
