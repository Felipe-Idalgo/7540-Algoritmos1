CC=gcc
CFLAGS=-std=c99 -Wall -Wconversion -Werror -lm

ENTREGA=*.c
COMPILABLES=*.c
32bit=32bit/utiles.o
64bit=64bit/utiles.o
EXEC=defendiendo

prueba: comandos.c
	$(CC) $(CFLAGS) comandos.c -o $(EXEC)

construccion_32: $(ENTREGA)
	$(CC) $(CFLAGS) $(COMPILABLES) $(32bit) -o $(EXEC)
	echo "Ingrese ./$(EXEC) para ejecutar"

construccion_64: $(ENTREGA)
	$(CC) $(CFLAGS) $(COMPILABLES) $(64bit) -o $(EXEC)
	echo "Ingrese ./$(EXEC) para ejecutar"

entrega: $(ENTREGA)
	zip entrega.zip $(ENTREGA)

.PHONY: clean

clean:
	rm $(EXEC)
	rm *.zip
