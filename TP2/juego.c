#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "defendiendo_torres.h"
#include "animos.h"
#include "utiles.h"

#define ENANOS 'G'
#define ELFOS 'L'
#define PRIMER_NIVEL 1
#define SEGUND_NIVEL 2
#define TERCER_NIVEL 3
#define CUARTO_NIVEL 4
#define JUGANDO 0
#define GANADO 1
#define AGREGADO 0

const char IGNORAR = 'X';
const float DELAY = 0.5;
const float DELAY_LARGO = 1.5;

const int ENANOS_INICIALES_PRIMER_NIVEL = 5;
const int ELFOS_INICIALES_PRIMER_NIVEL = 0;
const int ENANOS_INICIALES_SEGUND_NIVEL = 0;
const int ELFOS_INICIALES_SEGUND_NIVEL = 5;
const int ENANOS_INICIALES_TERCER_NIVEL = 3;
const int ELFOS_INICIALES_TERCER_NIVEL = 3;
const int ENANOS_INICIALES_CUARTO_NIVEL = 3;
const int ELFOS_INICIALES_CUARTO_NIVEL = 3;
const int COSTE_EXTRA = 50;
const int ORCOS_PARA_AGREGAR_ENANO = 25;
const int ORCOS_PARA_AGREGAR_DEFENSOR = 50;

const int MAX_ENEMIGOS_PRIMER_NIVEL = 100;
const int MAX_ENEMIGOS_SEGUND_NIVEL = 200;
const int MAX_ENEMIGOS_TERCER_NIVEL = 300;
const int MAX_ENEMIGOS_CUARTO_NIVEL = 500;


const int SIN_CAMINO = 0;
const int SIN_DEFENSORES = 0;
const int SIN_ENEMIGOS = 0;



//~ Pre: Recibe un juego inicializado.
//~ Pos: Devuelve true cuando se esta jugando el PRIMER_NIVEL.
bool primer_nivel(juego_t juego) {
    return juego.nivel_actual == PRIMER_NIVEL;
}
//~ Pre: Recibe un juego inicializado.
//~ Pos: Devuelve true cuando se esta jugando el SEGUND_NIVEL.
bool segundo_nivel(juego_t juego) {
    return juego.nivel_actual == SEGUND_NIVEL;
}
//~ Pre: Recibe un juego inicializado.
//~ Pos: Devuelve true cuando se esta jugando el TERCER_NIVEL.
bool tercer_nivel(juego_t juego) {
    return juego.nivel_actual == TERCER_NIVEL;
}
//~ Pre: Recibe un juego inicializado.
//~ Pos: Devuelve true cuando se esta jugando el CUARTO_NIVEL.
bool cuarto_nivel(juego_t juego) {
    return juego.nivel_actual == CUARTO_NIVEL;
}



//~ Pre: Recibe un numero de fila y un numero de columna de una matriz.
//~ Pos: Inicializa una coordenada para poder represer la posicion en la matriz.
void ubicar(coordenada_t *coordenada, int fil, int col){
    (*coordenada).fil = fil;
    (*coordenada).col = col;
}



//~ Pre: Recibe el numero de nivel.
//~ Pos: Obtiene los minimos y maximos de los valores que puede tomar una coordenada.
void conseguir_rango(int *min, int *max, int nivel) {
    *min = 0;
    if (nivel == PRIMER_NIVEL || nivel == SEGUND_NIVEL) {
            *max = 14;
    } else {
            *max = 19;
    }
}



//~ Pre: Recibe un numero ingresado por el usuario.
//~ Pos: Devuelve true si se encuentra entre los valores válidos que puede tomar una coordenada.
bool esta_en_rango(int num, int min, int max) {
    return (num >= min && num <= max);
}



//~ Pre: Recibe un numero de nivel y una coordenada sin inicializar.
//~ Pos: Inicializa la coordenada con valores validos ingresados por el usuario, en función del nivel que se este jugando.
void pedir_coordenada(coordenada_t *coordenada, int nivel) {
    int fil, col, min, max;
    conseguir_rango(&min, &max, nivel);

    printf("\nIngrese fila: ");
    scanf("%i", &fil);
    while (!esta_en_rango(fil, min, max)) {
        printf("No esta en rango! Vuleva a ingresar una fila: ");
        scanf("%i", &fil);
    }
    printf("Ingrese columna: ");
    scanf("%i", &col);
    while (!esta_en_rango(col, min, max)) {
        printf("No esta en rango! Vuleva a ingresar una columna: ");
        scanf("%i", &col);
    }

    ubicar(coordenada, fil, col);
}



//~ Pre: Recibe un juego en un nuevo nivel.
//~ Pos: Ubica la entrada los enemigos que atacaran a la torre 1 y la ubicacion de esta. Genera el camino 1.
void obtener_camino_1(juego_t *juego) {
    coordenada_t entrada_1, torre_1;

    if (primer_nivel(*juego)) {
            ubicar(&entrada_1, 5, 14);
            ubicar(&torre_1, 5, 0);

    } else if (segundo_nivel(*juego)) {
        (*juego).nivel.tope_camino_1 = SIN_CAMINO;
        return;

    } else if (tercer_nivel(*juego)) {
            ubicar(&entrada_1, 0, 16);
            ubicar(&torre_1, 19, 16);

    } else {
            ubicar(&entrada_1, 19, 16);
            ubicar(&torre_1, 0, 16);
    }
    obtener_camino((*juego).nivel.camino_1, &((*juego).nivel.tope_camino_1), entrada_1, torre_1);
}



//~ Pre: Recibe un juego en un nuevo nivel.
//~ Pos: Ubica la entrada los enemigos que atacaran a la torre 2 y la ubicacion de esta. Genera el camino 2.
void obtener_camino_2(juego_t *juego) {
    coordenada_t entrada_2, torre_2;

    if (primer_nivel(*juego)) {
        (*juego).nivel.tope_camino_2 = SIN_CAMINO;
        return;

    } else if (segundo_nivel(*juego)) {
            ubicar(&entrada_2, 5, 0);
            ubicar(&torre_2, 5, 14);

    } else if (tercer_nivel(*juego)) {
            ubicar(&entrada_2, 0, 4);
            ubicar(&torre_2, 19, 4);

    } else {
            ubicar(&entrada_2, 19, 4);
            ubicar(&torre_2, 0, 4);
    }
    obtener_camino((*juego).nivel.camino_2, &((*juego).nivel.tope_camino_2), entrada_2, torre_2);
}



//~ Pre: Recibe un juego en un nuevo nivel por inicializar, la cantidad de defensores de un tipo y el tipo de defensores que deben añadirse.
//~ Pos: Le permite al usuario elegir dónde ubicar a los defensores hasta cumplir con el número de defensores iniciales.
void agregar_defensores_iniciales(juego_t *juego, int cantidad, char tipo) {
    int i = 0;

    while (i < cantidad) {
        coordenada_t posicion;
        mostrar_juego(*juego);
        printf("\nVa a agregar un %c\n", tipo);
        pedir_coordenada(&posicion, (*juego).nivel_actual);
        if (agregar_defensor(&((*juego).nivel), posicion, tipo) == AGREGADO) {
            i++;
        } else {
            printf("\nLa posición está ocupada\n");
            detener_el_tiempo(DELAY_LARGO);
        }
    }
}



//~ Pre: Recibe un juego en nuevo nivel por inicializar y las indicaciones de defensores iniciales por añadirse.
//~ Pos: Inicializa todos los defensores iniciales.
void cargar_defensores_iniciales(juego_t *juego) {
    (*juego).nivel.tope_defensores = SIN_DEFENSORES;
    int enanos_iniciales, elfos_iniciales;

    if (primer_nivel(*juego)) {
        enanos_iniciales = ENANOS_INICIALES_PRIMER_NIVEL;
        elfos_iniciales = ELFOS_INICIALES_PRIMER_NIVEL;
    } else if (segundo_nivel(*juego)) {
        enanos_iniciales = ENANOS_INICIALES_SEGUND_NIVEL;
        elfos_iniciales = ELFOS_INICIALES_SEGUND_NIVEL;
    } else if (tercer_nivel(*juego)){
        enanos_iniciales = ENANOS_INICIALES_TERCER_NIVEL;
        elfos_iniciales = ELFOS_INICIALES_TERCER_NIVEL;
    } else {
        enanos_iniciales = ENANOS_INICIALES_CUARTO_NIVEL;
        elfos_iniciales = ELFOS_INICIALES_CUARTO_NIVEL;
    }
    agregar_defensores_iniciales(juego, enanos_iniciales, ENANOS);
    agregar_defensores_iniciales(juego, elfos_iniciales, ELFOS);
}





bool puede_agregar_enanos(juego_t juego) {
    return (!segundo_nivel(juego)) && (juego.torres.resistencia_torre_1 > COSTE_EXTRA) && (juego.torres.enanos_extra > SIN_DEFENSORES);
}





bool puede_agregar_elfos(juego_t juego) {
    return (!primer_nivel(juego)) && (juego.torres.resistencia_torre_2 > COSTE_EXTRA) && (juego.torres.elfos_extra > SIN_DEFENSORES);
}





bool hay_extra_disponible(juego_t juego) {
    return puede_agregar_enanos(juego) || puede_agregar_elfos(juego);
}





bool se_puede_agregar_extra(juego_t juego) {
    bool primera_condicion = juego.nivel.tope_enemigos > 0;
    bool segunda_condicion;
    if (primer_nivel(juego)) {
        segunda_condicion = juego.nivel.tope_enemigos % ORCOS_PARA_AGREGAR_ENANO == 0;
    } else {
        segunda_condicion = juego.nivel.tope_enemigos % ORCOS_PARA_AGREGAR_DEFENSOR == 0;
    }
    return (primera_condicion && segunda_condicion);
}





bool es_tipo_valido(char tipo) {
    return tipo == ENANOS || tipo == ELFOS;
}




void pedir_tipo(juego_t juego, char *tipo) {
    printf("\nPodes agregar un defensor de las tropas de");
    if (puede_agregar_enanos(juego)) {
        printf(" %c (%i)", ENANOS, juego.torres.enanos_extra);
    }
    if (puede_agregar_elfos(juego)) {
        if (puede_agregar_enanos(juego)) printf(" o de las tropas de");
        printf(" %c (%i)", ELFOS, juego.torres.elfos_extra);
    }
    printf("\nCostará %i a su torre. \nAgregar %c o %c, %c para ignorar: ", COSTE_EXTRA, ENANOS, ELFOS, IGNORAR);
    scanf(" %c", tipo);
    while (!(*tipo == IGNORAR) || !es_tipo_valido(*tipo)) {
        printf("El defensor puede ser %c o %c: ", ENANOS, ELFOS);
        scanf(" %c", tipo);
    }
}



//~ Pre: Recibe un juego cuando es permitido agregar un defensor extra de las torres.
//~ Pos: Agrega un defensor extra al juego y lo descuenta de las torres.
void agregar_defensor_extra(juego_t *juego) {
    char tipo;
    coordenada_t posicion;

    if (hay_extra_disponible(*juego)) {
        pedir_tipo(*juego, &tipo);
        if (tipo == IGNORAR) {
            printf("\nNo se han agregado defensores.\n");
            return;
        }

        pedir_coordenada(&posicion, (*juego).nivel_actual);
        while (agregar_defensor(&((*juego).nivel), posicion, tipo) != AGREGADO) {
            printf("\nLa posición está ocupada\n");
            pedir_coordenada(&posicion, (*juego).nivel_actual);
        }

        if (tipo == ENANOS) (*juego).torres.enanos_extra--;
        else (*juego).torres.elfos_extra--;

    } else {
        printf("\nNo te quedan defensores extra :(\n");
    }
}




//~ Pre: Recibe un juego en un nuevo nivel por inicializar.
//~ Pos: Elimina los enemigos que tenga de una instancia anterior y define max_enemigos_nivel de nivel.
void cargar_enemigos(juego_t *juego){
    (*juego).nivel.tope_enemigos = SIN_ENEMIGOS;

    if (primer_nivel(*juego)) {
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_PRIMER_NIVEL;
    } else if (segundo_nivel(*juego)) {
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_SEGUND_NIVEL;
    } else if (tercer_nivel(*juego)) {
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_TERCER_NIVEL;
    } else {
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_CUARTO_NIVEL;
    }
}




void mostrar_intro(juego_t juego) {
    if (primer_nivel(juego)) {
        printf("\nLos enemigos vienen por el este! \nPlanean atacar la torre 1. \nPosicione %i defensores de las tropas de %c para defenderlas\n", ENANOS_INICIALES_PRIMER_NIVEL, ENANOS);
    } else if (segundo_nivel(juego)) {
        printf("\nHay más tropas! \nAhora se acercan por el oeste para atacar la torre 2! \nPosicione %i defensores de las tropas de %c para defenderlas\n", ELFOS_INICIALES_SEGUND_NIVEL, ELFOS);
    } else if (tercer_nivel(juego)) {
        printf("\nOh no! Los enemigos siguen viviendo ahora por el norte. \nPueden atacar las dos torres. Puede posicionar %i defensores de las tropas de %c y %i de %c para defenderlas!\n", ENANOS_INICIALES_TERCER_NIVEL, ENANOS, ELFOS_INICIALES_TERCER_NIVEL, ELFOS);
    } else {
        printf("\nMás?!!! Los enemigos vienen por el sur! \n Se dirigen a ambas torres de nuevo. Ubique %i defensores de las tropas de %c y %i de %c para defenderlas\n", ENANOS_INICIALES_CUARTO_NIVEL, ENANOS, ELFOS_INICIALES_CUARTO_NIVEL, ELFOS);
    }
    printf("\nPresione [enter] para seguir!");
    getchar();
}



//~ Pre: Recibe un juego en una nueva instancia
//~ Pos: Inicializa un nuevo nivel modificando caminos, renovando defensores y eliminando enemigos previos
void cargar_nivel(juego_t *juego) {
    mostrar_intro(*juego);
    obtener_camino_1(juego);
    obtener_camino_2(juego);
    cargar_enemigos(juego);
    //~ cargar_defensores_iniciales(juego);
}



//~ Pre: Recibe el estado del juego una vez finalizado
//~ Pos: Muestra un mensaje segun el estado GANADO o PERDIDO
void mostrar_mensaje_final(int estado_juego) {
    if (estado_juego == GANADO) {
        printf("FELICIDADES! HAS COMPLETADO EL JUEGO CON ÉXITO\n");
    } else {
        printf("Una de las torres ha caído...\nGame over\n");
    }
}



int main() {
    system("clear");
    srand((unsigned)time(NULL));
    int viento=75;
    int humedad=75;
    char animo_legolas='M';
    char animo_gimli='M';
    juego_t juego;

    //~ animos(&viento, &humedad, &animo_legolas, &animo_gimli);
    inicializar_juego(&juego, viento, humedad, animo_legolas, animo_gimli);
    cargar_nivel(&juego);

    //~ for (int n = 1; n < 5; n++) {
        //~ juego.nivel_actual = n;
        //~ cargar_nivel(&juego);
        //~ mostrar_juego(juego);
        //~ getchar();
    //~ }

    while (estado_juego(juego) == JUGANDO) {
        if (estado_nivel(juego.nivel) == GANADO) {
            juego.nivel_actual++;
            cargar_nivel(&juego);
        }
        detener_el_tiempo(DELAY);
        system("clear");

        if (se_puede_agregar_extra(juego)) {
            mostrar_juego(juego);
            agregar_defensor_extra(&juego);
            detener_el_tiempo(DELAY);
            system("clear");
            }
        mostrar_juego(juego);
        jugar_turno(&juego);
    }

    mostrar_mensaje_final(estado_juego(juego));
    return 0;
}
