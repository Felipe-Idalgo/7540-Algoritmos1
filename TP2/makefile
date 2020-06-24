CC=gcc
CFLAGS=-std=c99 -Wall -Wconversion -Werror -lm

PRUEBA=defendiendo_torres.c
ENTREGA=juego.c defendiendo_torres.h defendiendo_torres.c animos.h animos.c
COMPILABLES=juego.c defendiendo_torres.c animos.c utiles.o
EXEC=juego


build: $(ENTREGA)
	$(CC) $(CFLAGS) $(COMPILABLES) -o $(EXEC)

build_and_exec: build
	./$(EXEC)

prueba:
	zip prueba.zip $(PRUEBA)

entrega: $(ENTREGA)
	zip entrega.zip $(ENTREGA)

.PHONY: clean

clean:
	rm $(EXEC)
	rm *.zip
