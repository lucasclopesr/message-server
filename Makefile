IDIR=lib
CC=gcc
CFLAGS=-g -Wall -I $(IDIR) -lm

ODIR=out
SDIR=src

all:
	mkdir -p out && gcc -c -o out/locations.o src/locations.c -g -Wall -I lib -lm
	mkdir -p out && gcc -c -o out/utils.o src/utils.c -g -Wall -I lib -lm
	mkdir -p out && gcc -c -o out/server.o src/server.c -g -Wall -I lib -lm
	mkdir -p out && gcc -c -o out/client.o src/client.c -g -Wall -I lib -lm
	gcc -o server out/locations.o out/utils.o out/server.o -g -Wall -I lib -lm
	gcc -o client out/utils.o out/client.o -g -Wall -I lib -lm

.PHONY: clean

clean:
		rm -f $(ODIR)/*.o
		rm -f server client
