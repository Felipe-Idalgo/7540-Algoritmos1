#include "archivos.h"
#include "defendiendo_torres.h"
#include "configuracion.h"
#include "caminos.h"
#include "utiles.h"
#include <string.h>
#include <stdbool.h>

#define ERROR -1

#define ENTRADA 'E'
#define TORRE 'T'
#define FILA 'F'
#define COLUMNA 'C'
#define CAMINO_UNO 1
#define CAMINO_DOS 2
#define PRIMER_NIVEL 1
#define SEGUND_NIVEL 2
#define TERCER_NIVEL 3
#define CUARTO_NIVEL 4
#define CAMINOS_INDEFINIDOS "-1"

const char  IZQUIERDA = 'A', ABAJO = 'S', DERECHA = 'D', ARRIBA = 'W';

const char  FORMATO_NIVEL[] = "NIVEL=%i\n",
            FORMATO_CAMINO[] = "CAMINO=%i\n",
            FORMATO_COORDENADA[] = "%i;%i\n",
            ARCHIVO_TEMPORAL[] = "tmp";

const int   AL_ESTE = 14, AL_OESTE = 5;

const int   SIN_CAMINO = 0;

/*
 * Definido en caminos.h
 */
void ubicar(coordenada_t *coordenada, int fil, int col){
    coordenada->fil = fil;
    coordenada->col = col;
}

//~ Posiciona la entrada de los enemigos que atacaran a la torre 1 y a la torre. 
void ubicar_extremos_1(juego_t *juego, coordenada_t *entrada_1, coordenada_t *torre_1) {
    if (juego->nivel_actual == PRIMER_NIVEL) {
        ubicar(entrada_1, MAXIMO_MATRIZ_MENOR / 2, MAXIMO_MATRIZ_MENOR);
        ubicar(torre_1, MAXIMO_MATRIZ_MENOR / 2, MINIMO_MATRIZ);

    } else if (juego->nivel_actual == TERCER_NIVEL) {
        ubicar(entrada_1, MINIMO_MATRIZ, AL_ESTE);
        ubicar(torre_1, MAXIMO_MATRIZ_MAYOR, AL_ESTE);

    } else if (juego->nivel_actual == CUARTO_NIVEL) {
        ubicar(entrada_1, MAXIMO_MATRIZ_MAYOR, AL_ESTE);
        ubicar(torre_1, MINIMO_MATRIZ, AL_ESTE);

    } else juego->nivel.tope_camino_1 = 0;
}

//~ Posiciona la entrada de los enemigos que atacaran a la torre 2 y a la torre. 
void ubicar_extremos_2(juego_t *juego, coordenada_t *entrada_2, coordenada_t *torre_2) {
    if (juego->nivel_actual == SEGUND_NIVEL) {
        ubicar(entrada_2, MAXIMO_MATRIZ_MENOR / 2, MINIMO_MATRIZ);
        ubicar(torre_2, MAXIMO_MATRIZ_MENOR / 2, MAXIMO_MATRIZ_MENOR);

    } else if (juego->nivel_actual == TERCER_NIVEL) {
        ubicar(entrada_2, MINIMO_MATRIZ, AL_OESTE);
        ubicar(torre_2, MAXIMO_MATRIZ_MAYOR, AL_OESTE);

    } else if (juego->nivel_actual == CUARTO_NIVEL) {
        ubicar(entrada_2, MAXIMO_MATRIZ_MAYOR, AL_OESTE);
        ubicar(torre_2, MINIMO_MATRIZ, AL_OESTE);

    } else juego->nivel.tope_camino_2 = 0;
}

/*
 * Pre: Recibe dos coordenadas inicializadas
 * Pos: Devuelve true si ambas señalan la misma posición
 */
bool es_misma_coordenada(coordenada_t coord_1, coordenada_t coord_2) {
    return (coord_1.fil == coord_2.fil && coord_1.col == coord_2.col);
}

/*
 * Pre: Recibe una coordenada, y el nivel actual del juego.
 * Pos: Según el nivel que se esté, devolverá true si la coordenada forma parte
 *      del mapa.
 */
bool esta_en_mapa(coordenada_t coordenada, int nivel_actual) {
    if (nivel_actual == PRIMER_NIVEL || nivel_actual == SEGUND_NIVEL)
        return (coordenada.fil >= MINIMO_MATRIZ && coordenada.fil <= MAXIMO_MATRIZ_MENOR
            && coordenada.col >= MINIMO_MATRIZ && coordenada.col <= MAXIMO_MATRIZ_MENOR);

    return (coordenada.fil >= MINIMO_MATRIZ && coordenada.fil <= MAXIMO_MATRIZ_MAYOR
        && coordenada.col >= MINIMO_MATRIZ && coordenada.col <= MAXIMO_MATRIZ_MAYOR);
}

/*
 * Pre: Recibe la posición anterior, la coordenada que se acaba de desplazar y el nivel actual.
 * Pos: Devuelve true si no se vuelve a la posición anterior y si no se va del mapa.
 */
bool es_movimiento_valido(coordenada_t pos_actual, coordenada_t pos_anterior, int nivel_actual) {
    return (!es_misma_coordenada(pos_actual, pos_anterior) && esta_en_mapa(pos_actual, nivel_actual));
}

/*
 * Pre: Recibe una coordenada duplicada del último movimiento, la posición anterior
 *       en el camino, y el nivel el cual se está construyendo.
 * Pos: Permite mover la coordenada a una posición válida. En caso de que no sea
 *      válida, vuelve a preguntar la dirección del movimiento
 */
void avanzar(coordenada_t *coordenada, coordenada_t pos_anterior, int nivel_actual) {
    char movimiento;
    coordenada_t coord_aux = *coordenada;
    bool es_valido = true;
    printf("\nUse [%c] Izquierda, [%c] Abajo, [%c] Derecha, [%c] Arriba, para moverse:", IZQUIERDA, ABAJO, DERECHA, ARRIBA);
    scanf(" %c", &movimiento);
    if (movimiento == IZQUIERDA) {
        ubicar(coordenada, coordenada->fil, coordenada->col-1);
    } else if (movimiento == DERECHA) {
        ubicar(coordenada, coordenada->fil, coordenada->col+1);
    } else if (movimiento == ABAJO) {
        ubicar(coordenada, coordenada->fil+1, coordenada->col);
    } else if (movimiento == ARRIBA){
        ubicar(coordenada, coordenada->fil-1, coordenada->col);
    } else {
        es_valido = false;
    }
    if (!es_movimiento_valido(*coordenada, pos_anterior, nivel_actual) || !es_valido) {
        *coordenada = coord_aux;
        printf("No estás ingresando un movimiento válido.\n");
        avanzar(coordenada, pos_anterior, nivel_actual);
    }
}

/*
 * Recibe dos coordenadas por referencia y las intercambia.
 */
void intercambiar_coordenadas(coordenada_t *coord_1, coordenada_t *coord_2) {
    coordenada_t coord_aux = *coord_1;
    *coord_1 = *coord_2;
    *coord_2 = coord_aux;
}

/*
 * Pre: Recibe la estructura de un juego y el número del camino que se desee formar.
 * Pos: Se utilizarán juego para mostrarse el mapa. Se utilizarán las estructuras de
 *      los caminos 1 y 2 para formar los nuevos caminos.
 *      El camino no debería alcanzar la MAX_LONGITUD_CAMINO.
 */
void construir_camino(juego_t *juego, int camino) {
    int i = 2;
    system("clear");
    system("clear");
    mostrar_juego(*juego);
    if (camino == CAMINO_UNO) {
        juego->nivel.camino_1[i] = juego->nivel.camino_1[i-2];
        avanzar(&(juego->nivel.camino_1[i]), juego->nivel.camino_1[i-2], juego->nivel_actual);
        while (!es_misma_coordenada(juego->nivel.camino_1[i], juego->nivel.camino_1[i-1])
            && i < MAX_LONGITUD_CAMINO-1) {
            intercambiar_coordenadas(&(juego->nivel.camino_1[i]), &(juego->nivel.camino_1[i-1]));
            juego->nivel.tope_camino_1 = 1 + i++;
            system("clear");
            mostrar_juego(*juego);
            juego->nivel.camino_1[i] = juego->nivel.camino_1[i-2];
            avanzar(&(juego->nivel.camino_1[i]), juego->nivel.camino_1[i-3], juego->nivel_actual);
        }
        juego->nivel.tope_camino_1 = 1 + i++;
    } else {
        juego->nivel.camino_2[i] = juego->nivel.camino_2[i-2];
        avanzar(&(juego->nivel.camino_2[i]), juego->nivel.camino_2[i-2], juego->nivel_actual);
        while (!es_misma_coordenada(juego->nivel.camino_2[i], juego->nivel.camino_2[i-1])
            && i < MAX_LONGITUD_CAMINO-1) {
            intercambiar_coordenadas(&(juego->nivel.camino_2[i]), &(juego->nivel.camino_2[i-1]));
            juego->nivel.tope_camino_2 = 1 + i++;
            system("clear");
            mostrar_juego(*juego);
            juego->nivel.camino_2[i] = juego->nivel.camino_2[i-2];
            avanzar(&(juego->nivel.camino_2[i]), juego->nivel.camino_2[i-3], juego->nivel_actual);
        }
        juego->nivel.tope_camino_2 = 1 + i++;
    }
}

/*
 * Pre: Recibe la estructura de un juego con nivel_actual definido.
 * Pos: Ubica las entradas y las torres según el nivel y le permite al usuario
 *      construir los caminos que desee.
 */
void crear_camino(juego_t *juego) {
    juego->nivel.tope_camino_1 = 2;
    juego->nivel.tope_camino_2 = 2;
    ubicar_extremos_1(juego, &(juego->nivel.camino_1[0]), &(juego->nivel.camino_1[1]));
    ubicar_extremos_2(juego, &(juego->nivel.camino_2[0]), &(juego->nivel.camino_2[1]));
    if (juego->nivel_actual != SEGUND_NIVEL) {
        construir_camino(juego, CAMINO_UNO);
    }
    if (juego->nivel_actual != PRIMER_NIVEL) {
        construir_camino(juego, CAMINO_DOS);
    }
}

/*
 * Escribe los caminos de un nivel en un archivo temporal.
 * En cada nivel el archivo de abre al final y adjunta la información.
 */
int escribir_camino(juego_t juego) {
    FILE* archivo = fopen(ARCHIVO_TEMPORAL, ADJUNTO);
    if (!archivo) {
        printf("No se pueden grabar los caminos acá.");
        return ERROR;
    }

    fprintf(archivo, FORMATO_NIVEL, juego.nivel_actual);
    if (juego.nivel.tope_camino_1)
        fprintf(archivo, FORMATO_CAMINO, CAMINO_UNO);
    for (int i = 0; i < juego.nivel.tope_camino_1; i++)
        fprintf(archivo, FORMATO_COORDENADA, juego.nivel.camino_1[i].fil, juego.nivel.camino_1[i].col);

    if (juego.nivel.tope_camino_2)
        fprintf(archivo, FORMATO_CAMINO, CAMINO_DOS);
    for (int i = 0; i < juego.nivel.tope_camino_2; i++)
        fprintf(archivo, FORMATO_COORDENADA, juego.nivel.camino_2[i].fil, juego.nivel.camino_2[i].col);

    fclose(archivo);
    return !ERROR;
}

/*
 * Definido en caminos.h
 */
int crear_nuevos_caminos (char nuevos_caminos[MAX_ARCHIVO]) {
    FILE* archivo = fopen(nuevos_caminos, ESCRITURA);
    if (!archivo) return ERROR;
    juego_t juego;
    configuracion_t configuracion;
    cargar_configuracion(&configuracion, SIN_ARCHIVO);
    inicializar_juego(&juego, 0, 0, (char) 0, (char) 0, configuracion);
    for (int i = 1; i <= MAX_NIVELES; i++) {
        juego.nivel_actual = i;
        crear_camino(&juego);
        escribir_camino(juego);
    }
    fclose(archivo);
    rename(ARCHIVO_TEMPORAL, nuevos_caminos);
    return !ERROR;
}

/*
 * Pre: Recibe el nombre del archivo donde se encuentran los caminos a utilizar,
 *      y un juego que inicializa un nuevo nivel.
 * Pos: Se inicializan los caminos del nivel de que se va a jugar.
 */
int leer_camino(char caminos_config[MAX_ARCHIVO], juego_t *juego) {
    int nivel = 0, num_camino, tope;
    FILE *archivo = fopen(caminos_config, LECTURA);
    if (!archivo) return ERROR;

    char linea[MAX_LINEA];
    fscanf(archivo, "%s", linea);
    while (!feof(archivo) && (nivel <= juego->nivel_actual)) {
        if (tienen_misma_etiqueta(linea, FORMATO_NIVEL)) {
            sscanf(linea, FORMATO_NIVEL, &nivel);
            if (juego->nivel_actual == nivel) {
                juego->nivel.tope_camino_1 = 0;
                juego->nivel.tope_camino_2 = 0;
            }
        } else if (tienen_misma_etiqueta(linea, FORMATO_CAMINO) && juego->nivel_actual == nivel) {
            sscanf(linea, FORMATO_CAMINO, &num_camino);
            tope = 0;
        } else {            
            if (num_camino == CAMINO_UNO && juego->nivel_actual == nivel) {
                sscanf(linea, FORMATO_COORDENADA, &(juego->nivel.camino_1[tope].fil), &(juego->nivel.camino_1[tope].col));
                juego->nivel.tope_camino_1 = tope + 1;
            } else if (juego->nivel_actual == nivel){
                sscanf(linea, FORMATO_COORDENADA, &(juego->nivel.camino_2[tope].fil), &(juego->nivel.camino_2[tope].col));
                juego->nivel.tope_camino_2 = tope + 1;
            }
            tope++;
        }
        fscanf(archivo, "%s", linea);            
    }
    fclose(archivo);
    return !ERROR;
}

//~ Pre: Recibe un juego en un nuevo nivel por inicializar sin caminos predefinidos.
//~ Pos: Genera el camino 1.
void obtener_camino_1(juego_t *juego) {
    coordenada_t entrada_1, torre_1;
    ubicar_extremos_1(juego, &entrada_1, &torre_1);
    if (juego->nivel_actual != SEGUND_NIVEL)
        obtener_camino(juego->nivel.camino_1, &(juego->nivel.tope_camino_1), entrada_1, torre_1);
    else juego->nivel.tope_camino_1 = 0;
}

//~ Pre: Recibe un juego en un nuevo nivel por inicializar sin caminos predefinidos.
//~ Pos: Genera el camino 2.
void obtener_camino_2(juego_t *juego) {
    coordenada_t entrada_2, torre_2;
    ubicar_extremos_2(juego, &entrada_2, &torre_2);
    if (juego->nivel_actual != PRIMER_NIVEL)
        obtener_camino(juego->nivel.camino_2, &(juego->nivel.tope_camino_2), entrada_2, torre_2);
    else juego->nivel.tope_camino_2 = 0;
}

/*
 * Definido en caminos.h
 */
int cargar_caminos(juego_t *juego, char caminos_config[MAX_ARCHIVO]) {
    if (!strcmp(caminos_config, CAMINOS_INDEFINIDOS)) {
        obtener_camino_1(juego);
        obtener_camino_2(juego);
        return !ERROR;
    }
    return leer_camino(caminos_config, juego);
}
