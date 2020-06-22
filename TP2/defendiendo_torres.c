#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "defendiendo_torres.h"
#include "utiles.h"

#define ENANOS 'G'
#define ELFOS 'L'
#define ORCOS 'O'
#define ENTRADAS 'E'
#define TORRES 'T'
#define CAMINO ' '
#define CELDA_VACIA '`'

#define CAMINO_UNO 1
#define CAMINO_DOS 2

#define PRIMER_NIVEL 1
#define SEGUND_NIVEL 2
#define TERCER_NIVEL 3
#define CUARTO_NIVEL 4

#define AGREGADO 0
#define NO_AGREGADO -1
#define JUGANDO 0
#define GANADO 1
#define PERDIDO -1

const int TAMANIO_MENOR = 15;
const int TAMANIO_MAYOR = 20;

const int ELFOS_EXTRAS = 10;
const int ENANOS_EXTRAS = 10;

const int MAXIMO_RESISTENCIA = 600;
const int MINIMO_RESISTENCIA = 0;

const char BUENO = 'B';
const char REGULAR = 'R';
const char MALO = 'M';

const int CRITICO_BUENO = 25;
const int CRITICO_REGULAR = 10;
const int CRITICO_MALO = 0;

const int ATAQUE_ELFOS = 30;
const int ATAQUE_ENANOS = 60;
const int ADICIONAL_CRITICO = 40;

const int LIBRE = 1;
const int NO_LIBRE = 0;

const int POSICION_INICIAL = 0;
const int VIDA_INICIAL = 200;
const int VIDA_EXTRA = 100;
const int SIN_VIDA = 0;

const char ENCABEZADO_A[] = " =================================================\n";
const char ENCABEZADO_1[] = "                        ESTE\n";
const char ENCABEZADO_2[] = "                        OESTE\n";
const char ENCABEZADO_B[] = " ===============================================================\n";
const char ENCABEZADO_3[] = "                                NORTE\n";
const char ENCABEZADO_4[] = "                                SUR\n";



//~ Pos: Define los estados iniciales de las torres para un juego
void inicializar_torres(torres_t* torres) {
    (*torres).resistencia_torre_1 = MAXIMO_RESISTENCIA;
    (*torres).resistencia_torre_2 = MAXIMO_RESISTENCIA;
    (*torres).enanos_extra = ENANOS_EXTRAS;
    (*torres).elfos_extra = ELFOS_EXTRAS;
}



//~ Pre: Recibe el animo de uno de los lideres e informacion sobre los efectos en sus tropas.
//~ Pos: Devuelve el porcetaje de probabilidad de tener dar un ataque crítico
int critico_segun_animo(char animo) {
    int critico;
    if (animo == BUENO) {
        critico = CRITICO_BUENO;
    } else if (animo == REGULAR) {
        critico = CRITICO_REGULAR;
    } else {
        critico = CRITICO_MALO;
    }
    return critico;
}



//~ Pre: Recibe el estado del tiempo e informacion sobre los efectos en las tropas.
//~ Pos: Devuelve el porcentaje de fallo de un ataque
int fallo_segun_tiempo(int tiempo) {
    int fallo;
    fallo = tiempo / 2;
    return fallo;
}



//~ Definido en la firma
void inicializar_juego(juego_t* juego, int viento, int humedad, char animo_legolas, char animo_gimli) {
    inicializar_torres(&((*juego).torres));
    (*juego).critico_legolas = critico_segun_animo(animo_legolas);
    (*juego).critico_gimli = critico_segun_animo(animo_gimli);
    (*juego).fallo_legolas = fallo_segun_tiempo(viento);
    (*juego).fallo_gimli = fallo_segun_tiempo(humedad);
    (*juego).nivel_actual = PRIMER_NIVEL;
}



//~ Pre: Recibe el estado de las torres en cada turno
//~ Pos: Detecta cuando la resistenciade alguna alcanza valores nulo
bool torres_en_pie(torres_t torres){
    return (torres.resistencia_torre_1 > SIN_VIDA) && (torres.resistencia_torre_2 > SIN_VIDA);
}



//~ Pre: Recibe la cantidad de enemigos restantes en el nivel
//~ Pos: Devuelve true cuando todos los enemigos de un nivel fueron vencidos
bool nivel_superado(nivel_t nivel) {
    int i = 0, orcos_vivos = nivel.max_enemigos_nivel;
    while (i < nivel.tope_enemigos && orcos_vivos) {
        if (nivel.enemigos[i].vida <= SIN_VIDA) {
            orcos_vivos --;
        }
        i++;
    }
    return !orcos_vivos;
}



//~ Definido en defendiendo_torres.h
int estado_juego(juego_t juego) {
    if ( !(torres_en_pie(juego.torres))) {
        return PERDIDO;
    } else if ((juego.nivel_actual == CUARTO_NIVEL) && nivel_superado(juego.nivel)) {
        return GANADO;
    } else {
        return JUGANDO;
    }
}



//~ Definido en defendiendo_torres.h
int estado_nivel(nivel_t nivel) {
    if (nivel_superado(nivel)) {
        return GANADO;
    } else {
        return JUGANDO;
    }
}



//~ Pre: Recibe las posiciones de cada componente en el mapa
//~ Pos: Son acumuladas en un vector posiciones
void obtener_posiciones(nivel_t nivel, coordenada_t posiciones[], int tope_posiciones) {
    int i, j, k;

    for (i = 0; i < nivel.tope_camino_1; i++) {
        posiciones[i] = nivel.camino_1[i];
    }
    for (j = 0; j < nivel.tope_camino_2; j++) {
        posiciones[i+j] = nivel.camino_2[j];
    }
    for (k = 0; k < nivel.tope_defensores; k++) {
        posiciones[i+j+k] = nivel.defensores[k].posicion;
    }
};



//~ Pre: Recibe un nivel con estructuras válidas
//~ Pos: Devuelve la cantidad de posiciones que se encuentrar reservadas/ocupadas
int posiciones_reservadas(nivel_t nivel) {
    return (nivel.tope_camino_1 + nivel.tope_camino_2 + nivel.tope_defensores);
}





int comparacion_de_coordenadas(coordenada_t coor_1, coordenada_t coor_2) {
    if (coor_1.fil < coor_2.fil) {
        return -1;
    } else if (coor_1.fil > coor_2.fil) {
        return 1;
    } else {
        return (coor_2.col - coor_1.col);
    }
}


//~ Pre: Recibe un vector con las posiciones ocupadas y la cual se desee conocer su estado de disponibilidad
//~ Pos: Devuelve 0 cuando la posicion se encuentra disponible y -1 cuando esta ocupada
int posicion_disponible(coordenada_t posiciones[MAX_LONGITUD_CAMINO], int tope_posiciones, coordenada_t posicion) {
    int i=0;
    int estado = LIBRE;

    while ((i < tope_posiciones) && (estado==LIBRE)) {
        if (comparacion_de_coordenadas(posiciones[i], posicion) == 0) {
            estado = NO_LIBRE;
        }
        i++;
    }
    return estado;
}



//~ Pre: Recibe la posicion y el tipo de un nuevo defensor
//~ Pos: Inicializa al defensor con su posicion, su tipo y su fuerza_ataque
void inicializar_defensor(defensor_t *defensor, coordenada_t posicion, char tipo){
    (*defensor).posicion = posicion;
    (*defensor).tipo = tipo;
    if (tipo == ELFOS) {
        (*defensor).fuerza_ataque = ATAQUE_ELFOS;
    } else {
        (*defensor).fuerza_ataque = ATAQUE_ENANOS;
    }
}



//~ Definido en defendiendo_torres.h
int agregar_defensor(nivel_t* nivel, coordenada_t posicion, char tipo) {
    int tope_posiciones = posiciones_reservadas(*nivel);
    coordenada_t posiciones[tope_posiciones];
    obtener_posiciones(*nivel, posiciones, tope_posiciones);

    if (posicion_disponible(posiciones, tope_posiciones, posicion)) {
        inicializar_defensor(&((*nivel).defensores[(*nivel).tope_defensores]), posicion, tipo);
        (*nivel).tope_defensores++;
        return AGREGADO;
    }
    return NO_AGREGADO;
}



//~ Pre: recibe el numero de camino donde se va a agregar al enemigo
//~ Pos: inicializa a un enemigo en el camino, en la posicion inicial, con vida inicial y un extra al azar
void inicializar_enemigo(enemigo_t *enemigo, int num_camino) {
    (*enemigo).camino = num_camino;
    (*enemigo).pos_en_camino = POSICION_INICIAL;
    (*enemigo).vida = VIDA_INICIAL + rand() % VIDA_EXTRA;
}


//~ Pre: Se pasa un nivel_t nivel
//~ Pos: Se agrega a un enemigo en los caminos que disponga el nivel
void agregar_enemigo(nivel_t* nivel) {
    if ((*nivel).tope_camino_1 > 0) {
        inicializar_enemigo((&(*nivel).enemigos[(*nivel).tope_enemigos]), CAMINO_UNO);
        (*nivel).tope_enemigos++;
    }
    if ((*nivel).tope_camino_2 > 0) {
        inicializar_enemigo((&(*nivel).enemigos[(*nivel).tope_enemigos]), CAMINO_DOS);
        (*nivel).tope_enemigos++;
    }
}





bool orco_vivo(enemigo_t enemigo) {
    return enemigo.vida > SIN_VIDA;
}



//~ Pre: Recibe el registro de un enemigo, una referencia al camino de interes y un punto de este
//~ Pos: Devuelve true si el enemigo se encuentra en tal camino y en aquella posicion con vida
bool orco_vivo_en_camino(enemigo_t enemigo, int camino, int posicion){
    return orco_vivo(enemigo) && (enemigo.camino == camino && enemigo.pos_en_camino == posicion);
}





int elevar_al_cuadrado (int num) {
    return num * num;
}





int distancia_del_camino(defensor_t defensor, coordenada_t coordenada) {
    return (elevar_al_cuadrado(defensor.posicion.fil - coordenada.fil) + elevar_al_cuadrado(defensor.posicion.col - coordenada.col));
}





bool en_alcance_de_flecha(defensor_t defensor, coordenada_t coordenada) {
    return defensor.tipo == ELFOS && distancia_del_camino(defensor, coordenada) <= 9;
}






bool en_alcance_de_hacha(defensor_t defensor, coordenada_t coordenada) {
    return defensor.tipo == ENANOS && distancia_del_camino(defensor, coordenada) <= 2;
}





bool logra_atacar(int acierto, int fallo) {
    return (acierto >= fallo);
}





bool logra_ataque_critico(int acierto, int critico) {
    return (acierto <= critico);
}






void atacan_defensores(juego_t *juego) {
    int fallo, critico;
    int i, j, k;
    defensor_t def;
    coordenada_t pos;
    enemigo_t enem;

    for (i = 0; i < (*juego).nivel.tope_camino_1; i++) {
        pos = (*juego).nivel.camino_1[i];
        for (j = 0; j < (*juego).nivel.tope_defensores; j++) {
            def = (*juego).nivel.defensores[j];
            if (en_alcance_de_flecha(def, pos)) {
                for (k = 0; k < (*juego).nivel.tope_enemigos; k++) {
                    enem = (*juego).nivel.enemigos[k];
                    if (orco_vivo_en_camino(enem, CAMINO_UNO, i)) {
                        fallo = rand() % 100;
                        if (logra_atacar(fallo, (*juego).fallo_legolas)) {
                            printf("Fuss");
                            critico = rand() % 100;
                            (*juego).nivel.enemigos[k].vida -= def.fuerza_ataque;
                            if (logra_ataque_critico(critico, (*juego).critico_legolas)) {
                                (*juego).nivel.enemigos[k].vida -= ADICIONAL_CRITICO;
                            }
                        }
                    }
                }
            }
            if (en_alcance_de_hacha(def, pos)) {
                for (k = 0; k < (*juego).nivel.tope_enemigos; k++) {
                    enem = (*juego).nivel.enemigos[k];
                    if (orco_vivo_en_camino(enem, CAMINO_UNO, i)) {
                        fallo = rand() % 100;
                        if (logra_atacar(fallo, (*juego).fallo_gimli)) {
                            printf("Pam");
                            critico = rand() % 100;
                            (*juego).nivel.enemigos[k].vida -= def.fuerza_ataque;
                            if (logra_ataque_critico(critico, (*juego).critico_gimli)) {
                                (*juego).nivel.enemigos[k].vida -= ADICIONAL_CRITICO;
                            }
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < (*juego).nivel.tope_camino_2; i++) {
        pos = (*juego).nivel.camino_2[i];
        for (j = 0; j < (*juego).nivel.tope_defensores; j++) {
            def = (*juego).nivel.defensores[j];
            if (en_alcance_de_flecha(def, pos)) {
                for (k = 0; k < (*juego).nivel.tope_enemigos; k++) {
                    enem = (*juego).nivel.enemigos[k];
                    if (orco_vivo_en_camino(enem, CAMINO_DOS, i)) {
                        fallo = rand() % 100;
                        if (logra_atacar(fallo, (*juego).fallo_legolas)) {
                            printf("Fuss");
                            critico = rand() % 100;
                            (*juego).nivel.enemigos[k].vida -= def.fuerza_ataque;
                            if (logra_ataque_critico(critico, (*juego).critico_legolas)) {
                                (*juego).nivel.enemigos[k].vida -= ADICIONAL_CRITICO;
                            }
                        }
                    }
                }
            }
            if (en_alcance_de_hacha(def, pos)) {
                for (k = 0; k < (*juego).nivel.tope_enemigos; k++) {
                    enem = (*juego).nivel.enemigos[k];
                    if (orco_vivo_en_camino(enem, CAMINO_DOS, i)) {
                        fallo = rand() % 100;
                        if (logra_atacar(fallo, (*juego).fallo_gimli)) {
                            printf("Pam");
                            critico = rand() % 100;
                            (*juego).nivel.enemigos[k].vida -= def.fuerza_ataque;
                            if (logra_ataque_critico(critico, (*juego).critico_legolas)) {
                                (*juego).nivel.enemigos[k].vida -= ADICIONAL_CRITICO;
                            }
                        }
                    }
                }
            }
        }
    }
}



//~ Pre: recibe un enemigo y las estructuras de un nivel
//~ Pos: Devuelve true si el enemigo llega al final del camino, donde se encuentran una torre
bool orco_llega_a_torre(enemigo_t enemigo, nivel_t nivel){
    return ((orco_vivo_en_camino(enemigo, CAMINO_UNO, nivel.tope_camino_1)) || (orco_vivo_en_camino(enemigo, CAMINO_DOS, nivel.tope_camino_2)));
}




//~ Pos: Cuando el enemigo llega a la torre la ataca con toda su vida y muere
void atacan_orcos(enemigo_t *enemigo, torres_t *torres) {
    if ((*enemigo).camino == CAMINO_UNO) {
        (*torres).resistencia_torre_1 -= (*enemigo).vida;
    } else {
        (*torres).resistencia_torre_2 -= (*enemigo).vida;
    }
    (*enemigo).vida = 0;
}




//~ Pos: todos los enemigos con vida avanzan 1 posicion
void avanzan_orcos(juego_t *juego) {
    for (int i = 0; i < (*juego).nivel.tope_enemigos; i++) {
        if  ((*juego).nivel.enemigos[i].vida > SIN_VIDA){
            (*juego).nivel.enemigos[i].pos_en_camino++;
            if (orco_llega_a_torre((*juego).nivel.enemigos[i], (*juego).nivel)) {
                atacan_orcos(&((*juego).nivel.enemigos[i]), &(*juego).torres);
            }
        }
    }
}





void jugar_turno(juego_t* juego) {
    atacan_defensores(juego);
    //~ atacan_elfos(juego);
    //~ atacan_enanos(juego);
    avanzan_orcos(juego);
    if ((*juego).nivel.tope_enemigos < (*juego).nivel.max_enemigos_nivel) {
        agregar_enemigo(&((*juego).nivel));
    }
}



//~ Pos: Define las dimensiones del terreno en cada nivel
void definir_dimensiones(int nivel, int *tope_fil, int *tope_col) {
    if (nivel == PRIMER_NIVEL || nivel == SEGUND_NIVEL) {
        *tope_fil = TAMANIO_MENOR;
        *tope_col = TAMANIO_MENOR;
    } else {
        *tope_fil = TAMANIO_MAYOR;
        *tope_col = TAMANIO_MAYOR;
    }
}



//~ Pos: Crea una matriz que permitirá ubicar a los componentes
void generar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], int tope_fil, int tope_col) {
    int i, j;

    for (i = 0; i < tope_fil; i++) {
        for (j = 0; j < tope_col; j++) {
            mapa[i][j] = CELDA_VACIA;
        }
    }
}



//~ Pos:
void mostrar_columnas(int tope_col) {
    int j;

    printf("  |");
    for (j=0; j<tope_col; j++){
        if (j < 10) printf(" ");
        printf("%i|", j);
    }
    printf("\n");
}



//~ Pos: Reconstruye en dos dimensiones la matriz
void mostrar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], int tope_fil, int tope_col) {
    int i, j;

    mostrar_columnas(tope_col);
    for (i=0; i<tope_fil; i++) {
        if (i < 10) printf(" ");
        printf("%i|", i);
        for (j=0; j<tope_col; j++){
            printf("%c%c|", mapa[i][j], mapa[i][j]);
        }
        printf("%i\n", i);
    }
    mostrar_columnas(tope_col);
}



//~ Pos: ubica el recorrido del camino en el mapa
void pintar_camino(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_LONGITUD_CAMINO], int tope_camino, int num_camino) {
    int pos;

    mapa[camino[0].fil][camino[0].col] = ENTRADAS;
    mapa[camino[tope_camino-1].fil][camino[tope_camino-1].col] = TORRES;
    for (pos = 1; pos < tope_camino-1; pos++){
        mapa[camino[pos].fil][camino[pos].col] = CAMINO;
    }
}



//~ Pos: ubica a los enemigos vivos en el mapa
void pintar_enemigos(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_LONGITUD_CAMINO], int tope_camino, enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos, int num_camino) {
    int pos, i;

    for (pos = 1; pos < tope_camino-1; pos++){
        for (i = 0; i< tope_enemigos; i++) {
            if (orco_vivo_en_camino(enemigos[i], num_camino, pos)) {
                mapa[camino[pos].fil][camino[pos].col] = ORCOS;
            }
        }
    }
}



//~ Pos: ubica a los defensores en el mapa
void pintar_defensores(char mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel) {
    int i;
    coordenada_t posicion;

    for (i = 0; i < nivel.tope_defensores; i++) {
        posicion = nivel.defensores[i].posicion;
        mapa[posicion.fil][posicion.col] = nivel.defensores[i].tipo;
    }
}




void mostrar_encabezado(juego_t juego) {
    if (juego.nivel_actual == PRIMER_NIVEL || juego.nivel_actual == SEGUND_NIVEL) printf(ENCABEZADO_A);
    else printf(ENCABEZADO_B);

    if (juego.nivel_actual == PRIMER_NIVEL) printf(ENCABEZADO_1);
    else if (juego.nivel_actual == SEGUND_NIVEL) printf(ENCABEZADO_2);
    else if (juego.nivel_actual == TERCER_NIVEL) printf(ENCABEZADO_3);
    else printf(ENCABEZADO_4);

    if (juego.nivel_actual == PRIMER_NIVEL || juego.nivel_actual == SEGUND_NIVEL) printf(ENCABEZADO_A);
    else printf(ENCABEZADO_B);
}




void mostrar_estado(juego_t juego) {
    printf("\nResistencia Torre 2: %i \tResistencia Torre 1: %i\n", juego.torres.resistencia_torre_2, juego.torres.resistencia_torre_1);
    printf("Elfos extra: %i \tEnanos extra: %i\n", juego.torres.elfos_extra, juego.torres.enanos_extra);
    printf("Enemigos: %i \tEnemigos restantes: %i\n", juego.nivel.tope_enemigos, (juego.nivel.max_enemigos_nivel-juego.nivel.tope_enemigos));
}



//~ Definido en defendiendo_torres.h
void mostrar_juego(juego_t juego) {
    system("clear");
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    int tope_fil, tope_col;

    definir_dimensiones(juego.nivel_actual, &tope_fil, &tope_col);
    generar_mapa(mapa, tope_fil, tope_col);

    if (juego.nivel.tope_camino_1) pintar_camino(mapa, juego.nivel.camino_1, juego.nivel.tope_camino_1, CAMINO_UNO);
    if (juego.nivel.tope_camino_2)pintar_camino(mapa, juego.nivel.camino_2, juego.nivel.tope_camino_2, CAMINO_DOS);

    pintar_enemigos(mapa, juego.nivel.camino_1, juego.nivel.tope_camino_1, juego.nivel.enemigos, juego.nivel.tope_enemigos, CAMINO_UNO);
    pintar_enemigos(mapa, juego.nivel.camino_2, juego.nivel.tope_camino_2, juego.nivel.enemigos, juego.nivel.tope_enemigos, CAMINO_DOS);
    pintar_defensores(mapa, juego.nivel);
    printf("\n");

    mostrar_encabezado(juego);
    mostrar_mapa(mapa, tope_fil, tope_col);
    mostrar_estado(juego);
}
