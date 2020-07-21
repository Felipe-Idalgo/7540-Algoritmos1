#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "defendiendo_torres.h"

#define ENANOS 'G'
#define ELFOS 'L'
#define ORCOS 'O'
#define ENTRADAS 'E'
#define TORRES 'T'
#define CAMINO ' '
#define CELDA_VACIA '_'

#define PRIMER_NIVEL 1
#define SEGUND_NIVEL 2
#define TERCER_NIVEL 3
#define CUARTO_NIVEL 4

#define AGREGADO 0
#define NO_AGREGADO -1
#define JUGANDO 0
#define GANADO 1
#define PERDIDO -1

const int   TAMANIO_MENOR = 15, TAMANIO_MAYOR = 20;

const int   ELFOS_EXTRAS = 10, ENANOS_EXTRAS = 10;

const int   MAXIMO_RESISTENCIA = 600;

const char  BUENO = 'B', REGULAR = 'R', MALO = 'M';

const int   CRITICO_BUENO = 25, CRITICO_REGULAR = 10, CRITICO_MALO = 0;

const int   ATAQUE_ELFOS = 30, ATAQUE_ENANOS = 60, ADICIONAL_CRITICO = 40;

const int   NO_ATACO = 0, ATACO = 1;

const int   DISTANCIA_DE_ATAQUE_ELFOS = 3, DISTANCIA_DE_ATAQUE_ENANOS = 2;

const int   LIBRE = 1, NO_LIBRE = 0;

const int   CAMINO_UNO = 1, CAMINO_DOS = 2;

const int   POSICION_INICIAL = 0;

const int   VIDA_INICIAL = 200, VIDA_EXTRA = 100, SIN_VIDA = 0;

const char  ENCABEZADO_A[] = " ================================================\n",
            ENCABEZADO_1[] = "                        ESTE\n",
            ENCABEZADO_2[] = "                       OESTE\n",
            ENCABEZADO_B[] = " ===============================================================\n",
            ENCABEZADO_3[] = "                              NORTE\n",
            ENCABEZADO_4[] = "                               SUR\n";



//~ Pre: Recibe las torres de un juego sin inicializar
//~ Pos: Inicializa los valores de las torres en sus valores máximos.
void inicializar_torres(torres_t* torres) {
    if (torres->resistencia_torre_1 == INDEFINIDO) torres->resistencia_torre_1 = MAXIMO_RESISTENCIA;
    if (torres->resistencia_torre_2 == INDEFINIDO) torres->resistencia_torre_2 = MAXIMO_RESISTENCIA;
    if (torres->enanos_extra == INDEFINIDO) torres->enanos_extra = ENANOS_EXTRAS;
    if (torres->elfos_extra == INDEFINIDO) torres->elfos_extra = ELFOS_EXTRAS;
}



//~ Pre: Recibe el ánimo de uno de los lideres, pudiendo ser BUENO, REGULAR o MALO.
//~ Pos: Genera el porcetaje de probabilidad de dar un ataque crítico.
int critico_segun_animo(char animo) {
    if (animo == BUENO)
        return CRITICO_BUENO;
    else if (animo == REGULAR)
        return CRITICO_REGULAR;
    return CRITICO_MALO;
}



//~ Pre: Recibe el estado del tiempo, ya sea de la velocidad del viento o de la humedad.
//~ Pos: Genera el porcentaje de fallo de los defensores, afectados por las condiciones meteorologicas.
int fallo_segun_tiempo(int tiempo) {
    int fallo;
    fallo = tiempo / 2;
    return fallo;
}



//~ Definido en la defendiendo_torres.h
void inicializar_juego(juego_t* juego, int viento, int humedad, char animo_legolas, char animo_gimli, configuracion_t configuracion) {
    *juego = configuracion.juego;
    inicializar_torres(&(juego->torres));
    if (juego->critico_legolas == INDEFINIDO) juego->critico_legolas = critico_segun_animo(animo_legolas);
    if (juego->critico_gimli == INDEFINIDO) juego->critico_gimli = critico_segun_animo(animo_gimli);
    if (juego->fallo_legolas == INDEFINIDO) juego->fallo_legolas = fallo_segun_tiempo(viento);
    if (juego->fallo_gimli == INDEFINIDO) juego->fallo_gimli = fallo_segun_tiempo(humedad);
    juego->nivel_actual = PRIMER_NIVEL;
}



//~ Pre: Recibe el estado de las torres en cada turno.
//~ Pos: Detecta cuando la resistencia de alguna alcanza valores nulo.
bool torres_en_pie(torres_t torres){
    return torres.resistencia_torre_1 > SIN_VIDA && torres.resistencia_torre_2 > SIN_VIDA;
}



//~ Pre: Recibe un enemigo
//~ Pos: Devuelve true si todavía tiene vida
bool orco_vivo(enemigo_t enemigo) {
    return enemigo.vida > SIN_VIDA;
}



//~ Pre: Recibe un nivel con enemigos y la cantidad de max_enemigos_nivel que se espera vencer.
//~ Pos: Devuelve true cuando todos los enemigos de un nivel fueron vencidos.
bool nivel_superado(nivel_t nivel) {
    int i = 0, orcos_con_vida = nivel.max_enemigos_nivel;
    while (i < nivel.tope_enemigos && orcos_con_vida) {
        if (!orco_vivo(nivel.enemigos[i]))
            orcos_con_vida--;
        i++;
    }
    return !orcos_con_vida;
}



//~ Pre: Recibe un juego inicializado
//~ Pos: Devuelve true cuando se esta jugando el CUARTO_NIVEL y este se ha superado (todos los enemigos han sido vencidos)
bool ultimo_nivel_superado(juego_t juego) {
    return juego.nivel_actual == CUARTO_NIVEL && nivel_superado(juego.nivel);
}



//~ Definido en defendiendo_torres.h
int estado_juego(juego_t juego) {
    if (!torres_en_pie(juego.torres))
        return PERDIDO;
    else if (ultimo_nivel_superado(juego))
        return GANADO;
    return JUGANDO;
}



//~ Definido en defendiendo_torres.h
int estado_nivel(nivel_t nivel) {
    if (nivel_superado(nivel))
        return GANADO;
    return JUGANDO;
}



//~ Pre: Recibe las coordenadas de cada componente en el mapa, ya sean defensores existentes o caminos.
//~ Pos: Son acumuladas en un vector posiciones.
void obtener_posiciones(nivel_t nivel, coordenada_t posiciones[], int tope_posiciones) {
    int i, j, k;

    for (i = 0; i < nivel.tope_camino_1; i++)
        posiciones[i] = nivel.camino_1[i];

    for (j = 0; j < nivel.tope_camino_2; j++)
        posiciones[i+j] = nivel.camino_2[j];

    for (k = 0; k < nivel.tope_defensores; k++)
        posiciones[i+j+k] = nivel.defensores[k].posicion;
};



//~ Pre: Recibe un nivel con estructuras válidas.
//~ Pos: Devuelve la cantidad de posiciones que se encuentrar reservadas/ocupadas.
int posiciones_reservadas(nivel_t nivel) {
    return nivel.tope_camino_1 + nivel.tope_camino_2 + nivel.tope_defensores;
}



//~ Pre: Recibe dos coordenadas.
//~ Pos: Devuelve positivo si la primera coordenada tiene un valor mayor a la segunda coordenada, y negativo si es al revés. Compara primero filas y si coinciden, columnas. Devuelve 0 cuando son iguales.
int comparacion_de_coordenadas(coordenada_t coor_1, coordenada_t coor_2) {
    if (coor_1.fil < coor_2.fil)
        return -1;
    else if (coor_1.fil > coor_2.fil)
        return 1;
    else
        return coor_2.col - coor_1.col;
}


//~ Pre: Recibe un vector con las posiciones ocupadas y una coordenada la cual se desee conocer su estado de disponibilidad.
//~ Pos: Devuelve LIBRE cuando la posicion se encuentra disponible y NO_LIBRE cuando esta ocupada.
int posicion_disponible(coordenada_t posiciones[MAX_LONGITUD_CAMINO], int tope_posiciones, coordenada_t posicion) {
    int i = 0;
    int estado = LIBRE;

    while ((i < tope_posiciones) && (estado==LIBRE)) {
        if (!comparacion_de_coordenadas(posiciones[i], posicion))
            estado = NO_LIBRE;
        i++;
    }
    return estado;
}



//~ Pre: Recibe un tipo para un defensor.
//~ Pos: Devuelve true si es parte de los ENANOS.
bool es_enano(char tipo) {
    return tipo == ENANOS;
}



//~ Pre: Recibe un tipo para un defensor.
//~ Pos: Devuelve true si es parte de los ELFOS.
bool es_elfo(char tipo) {
    return tipo == ELFOS;
}



//~ Pre: Recibe la posicion y el tipo de un nuevo defensor.
//~ Pos: Inicializa al defensor en su posicion, con su tipo y su fuerza_ataque.
void inicializar_defensor(defensor_t *defensor, coordenada_t posicion, char tipo){
    defensor->posicion = posicion;
    defensor->tipo = tipo;
    if (es_elfo(tipo))
        defensor->fuerza_ataque = ATAQUE_ELFOS;
    else
        defensor->fuerza_ataque = ATAQUE_ENANOS;
}



//~ Definido en defendiendo_torres.h
int agregar_defensor(nivel_t* nivel, coordenada_t posicion, char tipo) {
    int tope_posiciones = posiciones_reservadas(*nivel);
    coordenada_t posiciones[tope_posiciones];
    obtener_posiciones(*nivel, posiciones, tope_posiciones);

    if (posicion_disponible(posiciones, tope_posiciones, posicion)) {
        inicializar_defensor(&(nivel->defensores[nivel->tope_defensores]), posicion, tipo);
        nivel->tope_defensores++;
        return AGREGADO;
    }
    return NO_AGREGADO;
}



//~ Pre: Recibe el número de camino donde se va a agregar al enemigo.
//~ Pos: Inicializa a un enemigo en uno de los camino, en la POSICION_INICIAL, con VIDA_INICIAL y un extra al azar.
void inicializar_enemigo(enemigo_t *enemigo, int num_camino) {
    enemigo->camino = num_camino;
    enemigo->pos_en_camino = POSICION_INICIAL;
    enemigo->vida = VIDA_INICIAL + (rand() % VIDA_EXTRA + 1);
}



//~ Pre: Se recibe una instancia de nivel.
//~ Pos: Se agrega un enemigo en los caminos que disponga el nivel.
void agregar_enemigo(nivel_t* nivel) {
    if (nivel->tope_camino_1) {
        inicializar_enemigo((&nivel->enemigos[nivel->tope_enemigos]), CAMINO_UNO);
        nivel->tope_enemigos++;
    }
    if (nivel->tope_camino_2) {
        inicializar_enemigo((&nivel->enemigos[nivel->tope_enemigos]), CAMINO_DOS);
        nivel->tope_enemigos++;
    }
}



//~ Pre: Recibe el registro de un enemigo, una referencia al camino de interes y un punto de este.
//~ Pos: Devuelve true si el enemigo se encuentra en tal camino, en aquella posicion y con vida.
bool orco_vivo_en_camino(enemigo_t enemigo, int camino, int posicion){
    return (
        orco_vivo(enemigo)
        && (enemigo.camino == camino && enemigo.pos_en_camino == posicion)
    );
}



//~ Eleva al cuadrado un número entero.
int elevado_al_cuadrado(int num) {
    return num * num;
}



//~ Pre: Recibe un defensor y una coordenada.
//~ Pos: Devuelve la distancia entre ambas posiciones, elevada al cuadrado.
int distancia_al_cuadrado(defensor_t defensor, coordenada_t coordenada) {
    return (elevado_al_cuadrado(defensor.posicion.fil - coordenada.fil) + elevado_al_cuadrado(defensor.posicion.col - coordenada.col));
}



//~ Pre: Recibe un defensor y una coordenada.
//~ Pos: Devuelve la suma de los valores absolutos de la diferencia de las coordenadas.
int distancia_manhattan(defensor_t defensor, coordenada_t coordenada) {
    return (abs(defensor.posicion.fil - coordenada.fil) + abs(defensor.posicion.col - coordenada.col));
}



//~ Pre: Recibe un defensor y una coordenada.
//~ Pos: Devuelve true si el defensor es_elfo y la coordenada se encuentra en el alcance de su ataque.
bool en_alcance_de_flecha(defensor_t defensor, coordenada_t coordenada) {
    return (
        es_elfo(defensor.tipo)
        && distancia_manhattan(defensor, coordenada) <= DISTANCIA_DE_ATAQUE_ELFOS
    );
}




//~ Pre: Recibe un defensor y una coordenada.
//~ Pos: Devuelve true si el defensor es_enano y la coordenada se encuentra en el alcance de su ataque.
bool en_alcance_de_hacha(defensor_t defensor, coordenada_t coordenada) {
    return (
        es_enano(defensor.tipo)
        && (distancia_al_cuadrado(defensor, coordenada) <= DISTANCIA_DE_ATAQUE_ENANOS)
    );
}



//~ Genera un número aleatorio entre 0 y 99.
int porcentaje_aleatorio() {
    return rand() % 100;
}



//~ Pre: Recibe la instancia de un juego, def indica un defensor del nivel, y acierto es la probabilidad de dar un ataque.
//~ Pos: Devuelve true si el acierto es mayor al grado de fallo que tiene según su tipo.
bool logra_atacar(juego_t juego, int def, int acierto) {
    return (
        (es_elfo(juego.nivel.defensores[def].tipo) && acierto > juego.fallo_legolas)
        || (es_enano(juego.nivel.defensores[def].tipo) && acierto > juego.fallo_gimli)
    );
}



//~ Pre: Recibe la instancia de un juego, def corresponde a un defensor, y recibe la probabilidad de dar un ataque critico.
//~ Pos: Devuelve true si el critico es compatible con la probabilidad que tiene segun su tipo.
bool logra_ataque_critico(juego_t juego, int def, int critico) {
    return (
        (es_elfo(juego.nivel.defensores[def].tipo) && critico < juego.critico_legolas)
        || (es_enano(juego.nivel.defensores[def].tipo) && critico < juego.critico_gimli)
    );
}



//~ Pre: Recibe la instancia de juego, la posicion de un defensor en el vector defensores, y la de un enemigo (que puede atacar) en el vector.
//~ Pos: Si el defensor acierta ataque, se le descuenta vida al enemigo. Puede dar un crítico.
void ataca_orco(juego_t *juego, int def, int enem) {
    int acierto, critico;
    acierto = porcentaje_aleatorio();
    critico = porcentaje_aleatorio();
    if (logra_atacar(*juego, def, acierto)) {
        juego->nivel.enemigos[enem].vida -= juego->nivel.defensores[def].fuerza_ataque;
        if (logra_ataque_critico(*juego, def, critico))
            juego->nivel.enemigos[enem].vida -= ADICIONAL_CRITICO;
    }
}



//~ Pre: Recibe la instancia de juego, un camino y a uno de los elfos defensores.
//~ Pos: Busca enemigos con vida cerca del elfo. Si lo encuentra, intentará atacarlo.
void elfo_ataca_camino(juego_t *juego, coordenada_t camino[MAX_LONGITUD_CAMINO], int tope_camino, int num_camino, int def) {
    int i = 0, enem;
    while (i < tope_camino) {
        if (en_alcance_de_flecha(juego->nivel.defensores[def], camino[i])) {
            enem = 0;
            while (enem < juego->nivel.tope_enemigos) {
                if (orco_vivo_en_camino(juego->nivel.enemigos[enem], num_camino, i))
                    ataca_orco(juego, def, enem);
                enem++;
            }
        }
        i++;
    }
}



//~ Pre: Recibe la instancia de juego, un camino y a uno de los enanos defensores.
//~ Pos: Busca enemigos con vida cerca del enano. Si lo encuentra, intentará atacarlo. Sólo ataca 1 vez.
void enano_ataca_camino(juego_t *juego, coordenada_t camino[MAX_LONGITUD_CAMINO], int tope_camino, int num_camino, int def, int *ha_atacado) {
    int i = 0, enem;
    while (i < tope_camino && !(*ha_atacado)) {
        if (en_alcance_de_hacha(juego->nivel.defensores[def], camino[i])) {
            enem = 0;
            while (enem < juego->nivel.tope_enemigos && !(*ha_atacado)) {
                if (orco_vivo_en_camino(juego->nivel.enemigos[enem], num_camino, i)) {
                    ataca_orco(juego, def, enem);
                    *ha_atacado = ATACO;
                }
                enem++;
            }
        }
        i++;
    }
}



//~ Pre: Recibe la instancia de un juego y a uno de los elfos defensores.
//~ Pos: El elfo buscará enemigos en CAMINO_UNO y CAMINO_DOS. Atacará a todos los que estén a su alcance.
void ataca_elfo(juego_t *juego, int def) {
    elfo_ataca_camino(juego, juego->nivel.camino_1,juego->nivel.tope_camino_1, CAMINO_UNO, def);
    elfo_ataca_camino(juego, juego->nivel.camino_2,juego->nivel.tope_camino_2, CAMINO_DOS, def);
}



//~ Pre: Recibe la instancia de juego y a uno de los enanos defensores.
//~ Pos: El enano buscará enemigos en CAMINO_UNO y CAMINO_DOS. Sólo atacará al primero que encuentre a su alrededor.
void ataca_enano(juego_t *juego, int def) {
    int ataco = NO_ATACO;
    enano_ataca_camino(juego, juego->nivel.camino_1,juego->nivel.tope_camino_1, CAMINO_UNO, def, &ataco);
    enano_ataca_camino(juego, juego->nivel.camino_2,juego->nivel.tope_camino_2, CAMINO_DOS, def, &ataco);
}



//~ Pre: Recibe la instancia de juego y la posicion de un defensor en el vector defensores.
//~ Pos: Primero atacaran elfos y después enanos.
void atacan_defensores(juego_t *juego, int def) {
    if (def >= juego->nivel.tope_defensores) {
        return;
    } else if (es_elfo(juego->nivel.defensores[def].tipo)) {
        ataca_elfo(juego, def);
        atacan_defensores(juego, def+1);
    } else {
        atacan_defensores(juego, def+1);
        ataca_enano(juego, def);
    }
}



//~ Pre: Recibe un enemigo y las estructuras de un nivel.
//~ Pos: Devuelve true si el enemigo llega con vida al final del camino, donde se encuentran una torre.
bool orco_llega_a_torre(enemigo_t enemigo, nivel_t nivel){
    return (
        (orco_vivo_en_camino(enemigo, CAMINO_UNO, nivel.tope_camino_1-1))
        || (orco_vivo_en_camino(enemigo, CAMINO_DOS, nivel.tope_camino_2-1))
    );
}



//~ Pre: Recibe un enemigo en una torre.
//~ Pos: La ataca con toda su vida y muere.
void ataca_enemigo(enemigo_t *enemigo, torres_t *torres) {
    if (enemigo->camino == CAMINO_UNO)
        torres->resistencia_torre_1 -= enemigo->vida;
    else
        torres->resistencia_torre_2 -= enemigo->vida;
    enemigo->vida = SIN_VIDA;
}



//~ Todos los enemigos con vida avanzan 1 posición y si llega uno a la torre, la ataca.
void avanzan_enemigos(juego_t *juego) {
    int i;
    for (i = 0; i < juego->nivel.tope_enemigos; i++) {
        if (orco_vivo(juego->nivel.enemigos[i])){
            juego->nivel.enemigos[i].pos_en_camino++;
            if (orco_llega_a_torre(juego->nivel.enemigos[i], juego->nivel))
                ataca_enemigo(&(juego->nivel.enemigos[i]), &juego->torres);
        }
    }
}



//~ Definido en defendiendo_torres.h
void jugar_turno(juego_t* juego) {
    atacan_defensores(juego, 0);
    avanzan_enemigos(juego);
    if (juego->nivel.tope_enemigos < juego->nivel.max_enemigos_nivel)
        agregar_enemigo(&(juego->nivel));
}



//~ Define las dimensiones del terreno en cada nivel
void definir_dimensiones(int nivel, int *tope_fil, int *tope_col) {
    if (nivel == PRIMER_NIVEL || nivel == SEGUND_NIVEL) {
        *tope_fil = TAMANIO_MENOR;
        *tope_col = TAMANIO_MENOR;
    } else {
        *tope_fil = TAMANIO_MAYOR;
        *tope_col = TAMANIO_MAYOR;
    }
}



//~ Crea una matriz que permitirá ubicar a los componentes
void generar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], int tope_fil, int tope_col) {
    int i, j;

    for (i = 0; i < tope_fil; i++) {
        for (j = 0; j < tope_col; j++) {
            mapa[i][j] = CELDA_VACIA;
        }
    }
}



//~ Indica los números de las columnas de la matriz
void mostrar_columnas(int tope_col) {
    int j;

    printf("  |");
    for (j = 0; j < tope_col; j++) {
        if (j < 10) printf(" ");
        printf("%i|", j);
    }
    printf("\n");
}



//~ Imprime el contenido de la celda
void mostrar_celda(char celda) {
    printf("%c", celda);
    if (celda == CELDA_VACIA || celda == CAMINO) printf("%c", celda);
    else if (celda == ORCOS) printf("%c", CAMINO);
    else printf(".");
    printf("|");
}



//~ Reconstruye en dos dimensiones la matriz
void mostrar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], int tope_fil, int tope_col) {
    int i, j;

    mostrar_columnas(tope_col);
    for (i = 0; i < tope_fil; i++) {
        if (i < 10) printf(" ");
        printf("%i|", i);
        for (j = 0 ; j < tope_col; j++)
            mostrar_celda(mapa[i][j]);
        printf("%i\n", i);
    }
    mostrar_columnas(tope_col);
}



//~ Pre: Recibe una matriz "vacía" y uno de los caminos.
//~ Pos: Ubica el recorrido del camino en la matriz.
void pintar_camino(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_LONGITUD_CAMINO], int tope_camino, int num_camino) {
    int i;
    for (i = 0; i < tope_camino; i++)
        mapa[camino[i].fil][camino[i].col] = CAMINO;
}



//~ Pos: Ubica las entradas y torres de los caminos en la matriz.
void pintar_entradas_y_torres(char mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel) {
    coordenada_t entrada, torre;
    if (nivel.tope_camino_1) {
        entrada = nivel.camino_1[0];
        torre = nivel.camino_1[nivel.tope_camino_1-1];
        mapa[entrada.fil][entrada.col] = ENTRADAS;
        mapa[torre.fil][torre.col] = TORRES;
    }
    if (nivel.tope_camino_2) {
        entrada = nivel.camino_2[0];
        torre = nivel.camino_2[nivel.tope_camino_2-1];
        mapa[entrada.fil][entrada.col] = ENTRADAS;
        mapa[torre.fil][torre.col] = TORRES;
    }
}



//~ Pre: Recibe la matriz y la posicion de los enemigos en su camino.
//~ Pos: Ubica a los enemigos vivos en la matriz.
void pintar_enemigos(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_LONGITUD_CAMINO], int tope_camino, enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos, int num_camino) {
    int pos, i;

    for (pos = 0; pos < tope_camino; pos++){
        for (i = 0; i< tope_enemigos; i++) {
            if (orco_vivo_en_camino(enemigos[i], num_camino, pos))
                mapa[camino[pos].fil][camino[pos].col] = ORCOS;
        }
    }
}


//~ Pre: Recibe un nivel y una matriz.
//~ Pos: Ubica a los defensores en la matriz.
void pintar_defensores(char mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel) {
    int i;
    coordenada_t posicion;

    for (i = 0; i < nivel.tope_defensores; i++) {
        posicion = nivel.defensores[i].posicion;
        mapa[posicion.fil][posicion.col] = nivel.defensores[i].tipo;
    }
}



//~ Muestra un encabezado sobre el mapa.
void mostrar_encabezado(juego_t juego) {
    if (juego.nivel_actual == PRIMER_NIVEL || juego.nivel_actual == SEGUND_NIVEL)
        printf(ENCABEZADO_A);
    else
        printf(ENCABEZADO_B);

    if (juego.nivel_actual == PRIMER_NIVEL)
        printf(ENCABEZADO_1);
    else if (juego.nivel_actual == SEGUND_NIVEL)
        printf(ENCABEZADO_2);
    else if (juego.nivel_actual == TERCER_NIVEL)
        printf(ENCABEZADO_3);
    else
        printf(ENCABEZADO_4);

    if (juego.nivel_actual == PRIMER_NIVEL || juego.nivel_actual == SEGUND_NIVEL)
        printf(ENCABEZADO_A);
    else
        printf(ENCABEZADO_B);
}



//~ Muestra informacion sobre las torres, los defensores extra y los enemigos.
void mostrar_informacion(juego_t juego) {
    printf("\nResistencia Torre 2: %i \tResistencia Torre 1: %i\n", juego.torres.resistencia_torre_2, juego.torres.resistencia_torre_1);
    printf("Elfos extra: %i           \tEnanos extra: %i\n", juego.torres.elfos_extra, juego.torres.enanos_extra);
    printf("Enemigos: %i              \tEnemigos restantes: %i\n", juego.nivel.tope_enemigos, (juego.nivel.max_enemigos_nivel-juego.nivel.tope_enemigos));
}



//~ Definido en defendiendo_torres.h
void mostrar_juego(juego_t juego) {
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    int tope_fil, tope_col;

    definir_dimensiones(juego.nivel_actual, &tope_fil, &tope_col);
    generar_mapa(mapa, tope_fil, tope_col);

    pintar_camino(mapa, juego.nivel.camino_1, juego.nivel.tope_camino_1, CAMINO_UNO);
    pintar_camino(mapa, juego.nivel.camino_2, juego.nivel.tope_camino_2, CAMINO_DOS);
    pintar_enemigos(mapa, juego.nivel.camino_1, juego.nivel.tope_camino_1, juego.nivel.enemigos, juego.nivel.tope_enemigos, CAMINO_UNO);
    pintar_enemigos(mapa, juego.nivel.camino_2, juego.nivel.tope_camino_2, juego.nivel.enemigos, juego.nivel.tope_enemigos, CAMINO_DOS);
    pintar_entradas_y_torres(mapa, juego.nivel);
    pintar_defensores(mapa, juego.nivel);
    printf("\n");

    mostrar_encabezado(juego);
    mostrar_mapa(mapa, tope_fil, tope_col);
    mostrar_informacion(juego);
}
