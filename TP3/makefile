CC=gcc
CFLAGS=-std=c99 -Wall -Wconversion -Werror -lm

ENTREGA=*.c
COMPILABLES=*.c
32bit=32bit/utiles.o
64bit=64bit/utiles.o
EXEC=defendiendo

build_64: $(ENTREGA)
	$(CC) $(CFLAGS) $(COMPILABLES) $(64bit) -o $(EXEC)

build_32: $(ENTREGA)
	$(CC) $(CFLAGS) $(COMPILABLES) $(32bit) -o $(EXEC)

entrega: $(ENTREGA)
	zip entrega.zip $(ENTREGA)

.PHONY: clean

clean:
	rm $(EXEC)
	rm *.zip
