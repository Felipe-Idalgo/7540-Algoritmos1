CC=gcc
CFLAGS=-std=c99 -Wall -Wconversion -Werror -lm

COMPILABLES=*.c
32bit=32bit/utiles.o
64bit=64bit/utiles.o
EXEC=defendiendo

build_64:
	$(CC) $(CFLAGS) $(COMPILABLES) $(64bit) -o $(EXEC)

build_32:
	$(CC) $(CFLAGS) $(COMPILABLES) $(32bit) -o $(EXEC)

entrega:
	zip entrega.zip *

.PHONY: tmp clean

tmp:
	touch tmp.txt
	touch tmp.csv
	touch tmp.dat
	touch tmp.zip

clean: tmp
	rm $(EXEC)
	rm *.txt
	rm *.csv
	rm *.dat
	rm *.zip
