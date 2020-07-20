#define AUTOR "MG"
#define FECHA "Julio2020"

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

const int MINIMO_MATRIZ = 0;
const int MAXIMO_MATRIZ_MENOR = 14;
const int MAXIMO_MATRIZ_MAYOR = 19;
const int AL_ESTE = 14;
const int AL_OESTE = 5;

const int ENANOS_INICIALES_PRIMER_NIVEL = 5;
const int ELFOS_INICIALES_PRIMER_NIVEL = 0;
const int ENANOS_INICIALES_SEGUND_NIVEL = 0;
const int ELFOS_INICIALES_SEGUND_NIVEL = 5;
const int ENANOS_INICIALES_TERCER_NIVEL = 3;
const int ELFOS_INICIALES_TERCER_NIVEL = 3;
const int ENANOS_INICIALES_CUARTO_NIVEL = 4;
const int ELFOS_INICIALES_CUARTO_NIVEL = 4;
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



//~ Detiene el tiempo, limpia la consola y muestra el juego.
void limpiar_y_mostrar(juego_t juego, float velocidad) {
    detener_el_tiempo(velocidad);
    system("clear");
    mostrar_juego(juego);
}



//~ Se queda en espera hasta que el usuario ingrese una nueva línea.
void esperar() {
    printf("\n%c para seguir\n", IGNORAR);
    char nada_util;
    scanf(" %c", &nada_util);
}



//~ Primer mensaje. Le explica el juego al usuario.
void mostrar_ayuda() {
    system("clear");
    printf("\nVamos a jugar!\n");
    printf("\nContamos con enanos (G) y elfos (L) para defender las torres.\n");
    printf("\nLa torre 1 esta a cuidado de Gimli. Tiene 600 puntos de resistencia y en ella hay 10 enanos extra que te podran ayudar, si lo deseas.\n");
    printf("\nLa torre 2 esta a cuidado de Legolas. Tiene 600 puntos de resistencia y en ella hay 10 elfos extra que te podran ayudar, si los necesitas.\n");
    printf("\nLos Orcos se dirigen desde una Entrada a una sola de las Torres por un camino.\n");
    printf("\nUn G puede atacar a un enemigo por turno y solo aquellos que lo rodean.\n");
    printf("\nUn L puede atacar a varios enemigos por turno si están a 3 o menos celdas de distancia.\n");
    printf("\nLos ataques pueden fallar, como también pueden ser críticos o no.\n");
    printf("\nLos O no se detienen a luchar. Irán contra una de las torres y la atacarán con toda la vida restante que tengan.\n");
    printf("\nEl juego te habilitará momentos para pensar tu estrategia. Actúa sabiamente.\n");
    esperar();
}



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



//~ Pre: Recibe un numero de fila y un numero de columna para una matriz.
//~ Pos: Inicializa una coordenada.
void ubicar(coordenada_t *coordenada, int fil, int col){
    (*coordenada).fil = fil;
    (*coordenada).col = col;
}



//~ Pre: Recibe el numero de nivel y un rango sin inicializar.
//~ Pos: Obtiene los mínimos y máximos de los valores que puede tomar una coordenada en determinado nivel.
void conseguir_rango(int *min, int *max, int nivel) {
    *min = MINIMO_MATRIZ;
    if (nivel == PRIMER_NIVEL || nivel == SEGUND_NIVEL)
        *max = MAXIMO_MATRIZ_MENOR;
    else
        *max = MAXIMO_MATRIZ_MAYOR;
}



//~ Pre: Recibe un numero ingresado por el usuario y un rango adecuado al nivel.
//~ Pos: Devuelve true si se encuentra entre los valores válidos que puede tomar una coordenada.
bool esta_en_rango(int num, int min, int max) {
    return (num >= min && num <= max);
}



//~ Pre: Recibe un numero de nivel y una coordenada sin inicializar.
//~ Pos: Inicializa la coordenada con valores válidos ingresados por el usuario, adecuados al nivel del juego.
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



//~ Pre: Recibe un juego en un nuevo nivel por inicializar.
//~ Pos: Ubica la entrada los enemigos que atacaran a la torre 1 y la ubicación de esta. Genera el camino 1.
void obtener_camino_1(juego_t *juego, char camino[MAX_ARCHIVO]) {
    coordenada_t entrada_1, torre_1;

    if (primer_nivel(*juego)) {
        ubicar(&entrada_1, MAXIMO_MATRIZ_MENOR / 2, MAXIMO_MATRIZ_MENOR);
        ubicar(&torre_1, MAXIMO_MATRIZ_MENOR / 2, MINIMO_MATRIZ);

    } else if (segundo_nivel(*juego)) {
        (*juego).nivel.tope_camino_1 = SIN_CAMINO;
        return;

    } else if (tercer_nivel(*juego)) {
        ubicar(&entrada_1, MINIMO_MATRIZ, AL_ESTE);
        ubicar(&torre_1, MAXIMO_MATRIZ_MAYOR, AL_ESTE);

    } else {
        ubicar(&entrada_1, MAXIMO_MATRIZ_MAYOR, AL_ESTE);
        ubicar(&torre_1, MINIMO_MATRIZ, AL_ESTE);
    }
    obtener_camino((*juego).nivel.camino_1, &((*juego).nivel.tope_camino_1), entrada_1, torre_1);
}



//~ Pre: Recibe un juego en un nuevo nivel por inicializar.
//~ Pos: Ubica la entrada los enemigos que atacaran a la torre 2 y la ubicación de esta. Genera el camino 2.
void obtener_camino_2(juego_t *juego, char camino[MAX_ARCHIVO]) {
    coordenada_t entrada_2, torre_2;

    if (primer_nivel(*juego)) {
        (*juego).nivel.tope_camino_2 = SIN_CAMINO;
        return;

    } else if (segundo_nivel(*juego)) {
        ubicar(&entrada_2, MAXIMO_MATRIZ_MENOR / 2, MINIMO_MATRIZ);
        ubicar(&torre_2, MAXIMO_MATRIZ_MENOR / 2, MAXIMO_MATRIZ_MENOR);

    } else if (tercer_nivel(*juego)) {
        ubicar(&entrada_2, MINIMO_MATRIZ, AL_OESTE);
        ubicar(&torre_2, MAXIMO_MATRIZ_MAYOR, AL_OESTE);

    } else {
        ubicar(&entrada_2, MAXIMO_MATRIZ_MAYOR, AL_OESTE);
        ubicar(&torre_2, MINIMO_MATRIZ, AL_OESTE);
    }
    obtener_camino((*juego).nivel.camino_2, &((*juego).nivel.tope_camino_2), entrada_2, torre_2);
}



//~ Pre: Recibe un juego en un nuevo nivel por inicializar, la cantidad de defensores de un tipo y el tipo de defensores que deben añadirse.
//~ Pos: Le permite al usuario elegir dónde ubicar a los defensores hasta cumplir con el número de defensores iniciales.
void agregar_defensores_iniciales(juego_t *juego, int cantidad, char tipo) {
    int i = SIN_DEFENSORES;

    while (i < cantidad) {
        coordenada_t posicion;
        limpiar_y_mostrar(*juego, DELAY);
        printf("\nVa a agregar un %c\n", tipo);
        pedir_coordenada(&posicion, (*juego).nivel_actual);
        if (agregar_defensor(&((*juego).nivel), posicion, tipo) == AGREGADO)
            i++;
        else
            printf("\nLa posición está ocupada\n");
    }
}



//~ Pre: Recibe un juego en nuevo nivel por inicializar y las indicaciones de defensores iniciales para añadirse.
//~ Pos: Inicializa todos los defensores iniciales.
void cargar_defensores_iniciales(juego_t *juego, configuracion_t configuracion) {
    (*juego).nivel.tope_defensores = SIN_DEFENSORES;
    int enanos_iniciales = configuracion.enanos_inicio[(*juego).nivel_actual-1];
    int elfos_iniciales = configuracion.elfos_inicio[(*juego).nivel_actual-1];
    agregar_defensores_iniciales(juego, enanos_iniciales, ENANOS);
    agregar_defensores_iniciales(juego, elfos_iniciales, ELFOS);
}




//~ Pre: Recibe la instancia de un juego y la de sus torres.
//~ Pos: Si el nivel lo permite y las torres tienen suficientes recursos, permite agregar un enano al juego.
bool puede_agregar_enanos(juego_t juego, configuracion_t configuracion) {
    return (
        //~ (!segundo_nivel(juego)) &&
        (juego.torres.resistencia_torre_1 > configuracion.coste_enanos_torre_1)
        && (juego.torres.resistencia_torre_2 > configuracion.coste_enanos_torre_2)
        && (juego.torres.enanos_extra > SIN_DEFENSORES)
    );
}



//~ Pre: Recibe la instancia de un juego y la de sus torres.
//~ Pos: Si el nivel lo permite y las torres tienen suficientes recursos, permite agregar un elfo al juego.
bool puede_agregar_elfos(juego_t juego, configuracion_t configuracion) {
    return (
        //~ (!primer_nivel(juego)) &&
        (juego.torres.resistencia_torre_1 > configuracion.coste_elfos_torre_1)
        && (juego.torres.resistencia_torre_2 > configuracion.coste_elfos_torre_2)
        && (juego.torres.elfos_extra > SIN_DEFENSORES)
    );
}



//~ Pre: Recibe la instancia de un juego y la de sus torres.
//~ Pos: Devuelve verdadero si se permite agregar un elfo o un enano al juego.
bool hay_extra_disponible(juego_t juego, configuracion_t configuracion) {
    return puede_agregar_enanos(juego, configuracion) || puede_agregar_elfos(juego, configuracion);
}



//~ Pre: Recibe la cantidad de enemigos que salieron en el nivel y si quedan defensores extras.
//~ Pos: Devuelve verdadero si se dan todas las condiciones para agregar un defensor extra al juego.
bool se_puede_agregar_extra(juego_t juego, configuracion_t configuracion) {
    bool primera_condicion = hay_extra_disponible(juego, configuracion);
    bool segunda_condicion = juego.nivel.tope_enemigos > 0 && juego.nivel.tope_enemigos < juego.nivel.max_enemigos_nivel;
    bool tercera_condicion;
    if (primer_nivel(juego))
        tercera_condicion = juego.nivel.tope_enemigos % ORCOS_PARA_AGREGAR_ENANO == 0;
    else
        tercera_condicion = juego.nivel.tope_enemigos % ORCOS_PARA_AGREGAR_DEFENSOR == 0;

    return (primera_condicion && segunda_condicion && tercera_condicion);
}



//~ Pre: Recibe un tipo ingresado por el usuario.
//~ Pos: Devuelve verdadero si la respuesta es ENANOS, ELFOS o el usuario quiere IGNORAR el mensaje.
bool es_tipo_valido(char tipo, juego_t juego, configuracion_t configuracion) {
    return (
        (tipo == ENANOS && puede_agregar_enanos(juego, configuracion))
        || (tipo == ELFOS && puede_agregar_enanos(juego, configuracion))
        || (tipo == IGNORAR)
    );
}


//~ Pre: Recibe una instancia de juego y un tipo de defensor sin inicializar.
//~ Pos: El usuario ingresa un tipo valido, para agregar un defensor o ignorar el mensaje.
void pedir_tipo(juego_t juego, char *tipo, configuracion_t configuracion) {
    bool hay_enanos_disponibles = puede_agregar_enanos(juego, configuracion);
    bool hay_elfos_disponibles = puede_agregar_elfos(juego, configuracion);

    printf("\nPodes agregar un defensor de las tropas de");
    if (hay_enanos_disponibles)
        printf(" %c (%i)", ENANOS, juego.torres.enanos_extra);

    if (hay_elfos_disponibles) {
        if (hay_enanos_disponibles)
            printf(" o de las tropas de");
        printf(" %c (%i)", ELFOS, juego.torres.elfos_extra);
    }

    printf("\nRecuerda que agregar un %c costará %i a la Torre1 y %i a la Torre 2.\n", ENANOS, configuracion.coste_enanos_torre_1, configuracion.coste_enanos_torre_1);
    printf("\nAgregar un %c costará %i a la Torre1 y %i a la Torre 2. \n", ELFOS, configuracion.coste_elfos_torre_1, configuracion.coste_elfos_torre_2);
    printf("\nAgregar ");
    if (hay_enanos_disponibles) printf("%c/", ENANOS);
    if (hay_elfos_disponibles) printf("%c/", ELFOS);
    printf("%c(ignorar): ", IGNORAR);

    scanf(" %c", tipo);
    while (!es_tipo_valido(*tipo, juego, configuracion)) {
        printf("Intente de nuevo: ");
        if (hay_enanos_disponibles) printf("%c/", ENANOS);
        if (hay_elfos_disponibles) printf("%c/", ELFOS);
        printf("%c(ignorar): ", IGNORAR);
        scanf(" %c", tipo);
    }
}



//~ Elimina un enano de la torre_1.
void descontar_enano(torres_t *torres, configuracion_t configuracion) {
    (*torres).resistencia_torre_1 -= configuracion.coste_enanos_torre_1;
    (*torres).resistencia_torre_2 -= configuracion.coste_enanos_torre_2;
    (*torres).enanos_extra--;
}

//~ Elimina un elfo de la torre_2.
void descontar_elfo(torres_t *torres, configuracion_t configuracion) {
    (*torres).resistencia_torre_1 -= configuracion.coste_elfos_torre_1;
    (*torres).resistencia_torre_2 -= configuracion.coste_elfos_torre_2;
    (*torres).elfos_extra--;
}



//~ Pre: Recibe un juego cuando es permitido agregar un defensor extra de las torres.
//~ Pos: Agrega un defensor desde las torres al juego y lo descuenta de estas.
void agregar_defensor_extra(juego_t *juego, configuracion_t configuracion) {
    char tipo;
    coordenada_t posicion;

    pedir_tipo(*juego, &tipo, configuracion);
    if (tipo == IGNORAR) {
        printf("\nNo se han agregado defensores.\n");
        return;
    }

    pedir_coordenada(&posicion, (*juego).nivel_actual);
    while (agregar_defensor(&((*juego).nivel), posicion, tipo) != AGREGADO) {
        printf("\nLa posición está ocupada\n");
        pedir_coordenada(&posicion, (*juego).nivel_actual);
    }

    if (tipo == ENANOS) descontar_enano(&((*juego).torres), configuracion);
    else descontar_elfo(&((*juego).torres), configuracion);
}



//~ Pre: Recibe un juego en un nuevo nivel por inicializar.
//~ Pos: Elimina los enemigos de una instancia anterior y define max_enemigos_nivel de nivel.
void cargar_enemigos(juego_t *juego){
    (*juego).nivel.tope_enemigos = SIN_ENEMIGOS;

    if (primer_nivel(*juego))
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_PRIMER_NIVEL;
    else if (segundo_nivel(*juego))
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_SEGUND_NIVEL;
    else if (tercer_nivel(*juego))
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_TERCER_NIVEL;
    else
        (*juego).nivel.max_enemigos_nivel = MAX_ENEMIGOS_CUARTO_NIVEL;
}



//~ Selección de mensajes para mostrar al inicio de cada nivel.
void mostrar_intro_nivel(juego_t juego) {
    if (primer_nivel(juego)) {
        esperar();
        mostrar_ayuda(juego);
        printf("\nLos enemigos vienen por el este! \nPlanean atacar la torre 1. \nSe necesitan %i defensores de las tropas de %c para protegerla\n", ENANOS_INICIALES_PRIMER_NIVEL, ENANOS);
    } else if (segundo_nivel(juego)) {
        printf("\nVienen más orcos! \nAhora se acercan por el oeste para atacar la torre 2! \nPosicione %i defensores de las tropas de %c para detenerlos\n", ELFOS_INICIALES_SEGUND_NIVEL, ELFOS);
    } else if (tercer_nivel(juego)) {
        printf("\nOh no! Los enemigos siguen acercándose, esta vez por el norte. \nPueden atacar las dos torres! Ubique %i defensores de las tropas de %c y %i de %c para protegerlas!\n", ENANOS_INICIALES_TERCER_NIVEL, ENANOS, ELFOS_INICIALES_TERCER_NIVEL, ELFOS);
    } else {
        printf("\nTodavía hay más?!!! Los enemigos vienen por el sur! \nSe dirigen a ambas torres de nuevo. Ubique %i defensores de las tropas de %c y %i de %c para defenderlas\n", ENANOS_INICIALES_CUARTO_NIVEL, ENANOS, ELFOS_INICIALES_CUARTO_NIVEL, ELFOS);
    }
    esperar();
}



//~ Pre: Recibe un juego en una nueva instancia.
//~ Pos: Inicializa un nuevo nivel modificando caminos, renovando defensores y eliminando enemigos previos. Introduce el nivel con un mensaje.
void cargar_nivel(juego_t *juego, configuracion_t configuracion) {
    mostrar_intro_nivel(*juego);
    obtener_camino_1(juego, configuracion.caminos);
    obtener_camino_2(juego, configuracion.caminos);
    cargar_enemigos(juego);
    cargar_defensores_iniciales(juego, configuracion);
}



//~ Pre: Recibe el estado del juego una vez finalizado.
//~ Pos: Muestra un mensaje según el estado GANADO o PERDIDO.
void mostrar_mensaje_final(int estado_juego) {
    for (int i=MAXIMO_MATRIZ_MAYOR; i>=0; i--) {
        detener_el_tiempo(DELAY);
        system("clear");
        for (int j = i; j>=0; j--)
            printf("\n");
        if (estado_juego == GANADO)
            printf("FELICIDADES! HAS COMPLETADO EL JUEGO CON ÉXITO\n");
        else
            printf("Una de las torres ha caído...\nGame over\n");

        printf("\n\n\nGracias por jugar!!\n");
        printf("\n\nEscrito, producido y guionado por %s ;)\n", AUTOR);
        printf("\%s.\n", FECHA);
    }
}



int cantidad_de_enemigos_vencidos(juego_t juego) {
    int i, orcos_muertos = SIN_ENEMIGOS;

    for (i = 0; i < juego.nivel.tope_enemigos; i++)
        if (juego.nivel.enemigos[i].vida <= 0) orcos_muertos++;

    switch(juego.nivel_actual) {
        case CUARTO_NIVEL:
            orcos_muertos += MAX_ENEMIGOS_TERCER_NIVEL;
        case TERCER_NIVEL:
            orcos_muertos += MAX_ENEMIGOS_SEGUND_NIVEL;
        case SEGUND_NIVEL:
            orcos_muertos += MAX_ENEMIGOS_PRIMER_NIVEL;
        default:
            break;
    }
    return orcos_muertos;
}



int recursos_usados (torres_t torres, configuracion_t configuracion) {
    int i, recursos = torres.resistencia_torre_1;
    recursos += torres.resistencia_torre_2;
    recursos += torres.enanos_extra;
    recursos += torres.elfos_extra;
    for (i = 0; i < configuracion.max_niveles; i++) {
        recursos += configuracion.enanos_inicio[i];
        recursos += configuracion.elfos_inicio[i];
    }
    return recursos;
}



void grabar(juego_t juego, FILE* grabacion) {
    if (grabacion) fwrite(&juego, sizeof(juego_t), 1, grabacion);
}



void grabar_rank(juego_t juego, FILE* ranking) {
    if (ranking) {
        /*
         * insersion ordenada
         */
    };
}



void reproducir(FILE* grabacion, float velocidad) {
    juego_t juego;
    size_t leido = fread(&juego, sizeof(juego_t), 1, grabacion);
    while (leido) {
        limpiar_y_mostrar(juego, velocidad);
        leido = fread(&juego, sizeof(juego_t), 1, grabacion);
    }
}



void iniciar_juego(configuracion_t configuracion, FILE* grabacion, FILE* ranking) {
    srand((unsigned)time(NULL));
    int recursos, viento, humedad;
    char animo_legolas, animo_gimli;
    juego_t juego;
    rank_t rank;

    animos(&viento, &humedad, &animo_legolas, &animo_gimli);
    inicializar_juego(&juego, viento, humedad, animo_legolas, animo_gimli, configuracion);
    recursos = recursos_usados(juego.torres, configuracion);
    cargar_nivel(&juego, configuracion);
    grabar(juego, grabacion);

    while (estado_juego(juego) == JUGANDO) {
        if (estado_nivel(juego.nivel) == GANADO) {
            juego.nivel_actual++;
            cargar_nivel(&juego, configuracion);
        }
        limpiar_y_mostrar(juego, configuracion.velocidad);

        if (se_puede_agregar_extra(juego, configuracion)) {
            agregar_defensor_extra(&juego, configuracion);
            detener_el_tiempo(DELAY);
            limpiar_y_mostrar(juego, configuracion.velocidad);
            }
        jugar_turno(&juego);
        grabar(juego, grabacion);
    }

    rank.puntaje = (cantidad_de_enemigos_vencidos(juego) * 1000) / recursos;
    mostrar_mensaje_final(estado_juego(juego));
    printf("Lograste %i puntos\n", rank.puntaje);
    /*
     * grabar rank
     */
}
