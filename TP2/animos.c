#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "animos.h"

const char LEGOLAS = 'L';
const char GIMLI = 'G';

const int VELOCIDAD_VIENTO_PRIMER_PERIODO = 75;
const int VELOCIDAD_VIENTO_SEGUNDO_PERIODO = 50;
const int VELOCIDAD_VIENTO_TERCER_PERIODO = 25;
const int HUMEDAD_MANIANA = 75;
const int HUMEDAD_TARDE = 25;
const int HUMEDAD_NOCHE = 50;
const int HUMOR_PIE_DERECHO = 10;
const int HUMOR_PIE_IZQUIERDO = 0;
const int HUMOR_ENSALADA = 20;
const int HUMOR_HAMBURGUESA = 15;
const int HUMOR_PIZZA = 10;
const int HUMOR_GUISO = 5;
const int HUMOR_POCO_DESCANSO = 0;
const int HUMOR_SUFICIENTE_DESCANSO = 10;
const int HUMOR_GRAN_DESCANSO = 20;
const int HUMOR_MIN_MALO = 5;
const int HUMOR_MIN_REGULAR = 21;
const int HUMOR_MIN_BUENO = 36;
const int HUMOR_MAX = 50;
const char ANIMO_MALO = 'M';
const char ANIMO_REGULAR = 'R';
const char ANIMO_BUENO = 'B';

const int DIA_MIN_PRIMER_PERIODO = 1;
const int DIA_MIN_SEGUNDO_PERIODO = 11;
const int DIA_MIN_TERCER_PERIODO = 21;
const int DIA_MAX = 30;
const char MANIANA = 'M';
const char TARDE = 'T';
const char NOCHE = 'N';
const char PIE_IZQUIERDO = 'I';
const char PIE_DERECHO = 'D';
const char ENSALADA = 'E';
const char HAMBURGUESA = 'H';
const char PIZZA = 'P';
const char GUISO = 'G';
const int HORAS_MIN_POCO_DESCANSO = 0;
const int HORAS_MIN_SUFICIENTE_DESCANSO = 5;
const int HORAS_MIN_GRAN_DESCANSO = 9;
const int HORAS_MAX = 12;

//~ pre: letra tiene que ser inicial de LEGOLAS o GIMLI
//~ pos: devuelve true si es LEGOLAS, devuelve false si es GIMLI
bool es_legolas(char letra){
    return (letra == LEGOLAS);
}

//~ pre: dia representa una fecha ingresada por el usuario, DIA_MIN_PRIMER_PERIODO marca el inicio del periodo, DIA_MIN_SEGUNDO_PERIODO marca el fin de este (es excluyente) y el comienzo del siguiente
//~ pos: devolvera true si dia esta en el primer periodo, false si forma parte de otro periodo
bool es_primer_periodo(int dia) {
    return (dia >= DIA_MIN_PRIMER_PERIODO && dia < DIA_MIN_SEGUNDO_PERIODO);
}

//~ pre: dia representa una fecha ingresada por el usuario, DIA_MIN_SEGUNDO_PERIODO el comienzo del segundo periodo, DIA_MIN_TERCER_PERIODO marca el fin del periodo (es excluyente) y el comienzo del siguiente
//~ pos: devolvera true si pertenece al segundo periodo, false si esta fuera de rango
bool es_segundo_periodo(int dia) {
    return (dia >= DIA_MIN_SEGUNDO_PERIODO && dia < DIA_MIN_TERCER_PERIODO);
}

//~ pre: dia representa una fecha ingresada por el usuario, DIA_MIN_TERCER_PERIODO marca el comienzo del ultimo periodo, DIA_MAX marca el fin de este
//~ pos: devolvera true si pertenece al ultimo periodo, false si esta fuera de este
bool es_tercer_periodo(int dia) {
    return (dia >= DIA_MIN_TERCER_PERIODO && dia <= DIA_MAX);
}

//~ pre: dia es un numero ingresado por el usuario
//~ pos: devolvera true si representa un dia del mes (1 a 30), false si no
bool es_dia_de_mes(int dia) {
    return (es_primer_periodo(dia) || es_segundo_periodo(dia) || es_tercer_periodo(dia));
}

//~ pre: hora es inicial de un momento del dia ingresado por el usuario y MANIANA representa la mañana
//~ pos: devolvera true si el usuario indica que es de mañana, false si no
bool es_maniana(char hora) {
    return (hora == MANIANA);
}

//~ pre: hora es inicial de un momento del dia y TARDE representa la tarde
//~ pos: devolvera true si es la hora de la tarde
bool es_tarde(char hora) {
    return (hora == TARDE);
}

//~ pre: hora es inicial de un momento del dia y NOCHE es representa la de noche
//~ pos: devolvera true cuando la hora sea de noche
bool es_noche(char hora) {
    return (hora == NOCHE);
}

//~ pre: hora es una inicial
//~ pos: devolvera true si hora es inicial de mañana, tarde o noche
bool es_hora_de_dia(char hora) {
    return (es_maniana(hora) || es_tarde(hora) || es_noche(hora));
}

//~ pre: pie indica uno de los pies, PIE_DERECHO es representará el pie derecho
//~ pos: devolvera true si el pie es el derecho, false si no lo es
bool es_pie_derecho(char pie) {
    return (pie == PIE_DERECHO);
}

//~ pre: pie indica uno de los pies, PIE_IZQUIERDO es representa al pie izquierdo
//~ pos: devolverá true si el pie es el izquierdo, false si no lo es
bool es_pie_izquierdo(char pie) {
    return (pie == PIE_IZQUIERDO);
}

//~ pre: pie es un dato ingresado por el usuario
//~ pos: devolvera true si pie puede ser izquierdo o derecho, false si no es ninguno
bool es_pie_valido(char pie) {
    return (es_pie_derecho(pie) || es_pie_izquierdo(pie));
}

//~ pre: cena la inicial de una comida, ENSALADA es representativo de ensalada
//~ pos: devolvera true si la cena es ensalada, false si no
bool cena_ensalada(char cena) {
    return (cena == ENSALADA);
}

//~ pre: cena la inicial de una comida, HAMBURGUESA es representa la hamburguesa
//~ pos: devolvera true si la cena es hamburguesa, false si no
bool cena_hamburguesa(char cena) {
    return (cena == HAMBURGUESA);
}

//~ pre: cena es la inicial de una comida, PIZZA es representa la pizza
//~ pos: devolvera true si la cena fue pizza, false si no
bool cena_pizza(char cena) {
    return (cena == PIZZA);
}

//~ pre: cena es la inicial de una comida, GUISO es es la inicial de guiso
//~ pos: devolvera true si cenó guiso, false si no
bool cena_guiso(char cena) {
    return (cena == GUISO);
}

//~ pre: cena es la inicial de la ultima cena que tuvo LEGOLAS o GIMLI
//~ pos: devolverá true si cena se encuentra entre las posibles comidas de los elfos o enanos
bool es_cena_valida(char cena) {
    return (cena_ensalada(cena) || cena_hamburguesa(cena) || cena_pizza(cena) || cena_guiso(cena));
}

//~ pre: horas es la cantidad de horas que durmió, HORAS_MIN_POCO_DESCANSO es lo mínimo que se puede (o no) dormir, HORAS_MIN_SUFICIENTE_DESCANSO es máximo excluyente de haber tenido poco descanso
//~ pos: devolverá true si durmió poco o nada
bool es_poco_descanso(int horas) {
    return (horas >= HORAS_MIN_POCO_DESCANSO && horas < HORAS_MIN_SUFICIENTE_DESCANSO);
}

//~ pre: horas es la cantidad de horas que durmió, HORAS_MIN_SUFICIENTE_DESCANSO es lo mínimo para tener un buen sueño, HORAS_MIN_GRAN_DESCANSO es el tope de un buen sueño
//~ pos: devolverá true si descanso unas cuántas horas garantizandose un buen sueño, false si pudo haber dormido más, o menos.
bool es_suficiente_descanso(int horas) {
    return (horas >= HORAS_MIN_SUFICIENTE_DESCANSO && horas < HORAS_MIN_GRAN_DESCANSO);
}

//~ pre: horas es la cantidad de horas que durmió, HORAS_MIN_GRAN_DESCANSO es lo mínimo para considerarse que ha descansado mucho, HORAS_MAX es el tope de lo que se puede dormir en guerra
//~ pos: devolverá true si se siente renovado tras tener un gran sueño
bool es_gran_descanso(int horas) {
    return (horas >= HORAS_MIN_GRAN_DESCANSO && horas <= HORAS_MAX);
}

//~ pre: horas es un dato ingresado por el usuario que representa la cantidad de horas que durmió un lider
//~ pos: devolverá true si el dato se encuentra entre lo mínimo y lo máximo que puede dormir, false si el dato no puede corresponder a las horas de sueño
bool esta_en_el_rango_de_horas(int horas) {
    return (es_poco_descanso(horas) || es_suficiente_descanso(horas) || es_gran_descanso(horas));
}

//~ pre: dia es un número que se encuentra entre 1 y 30 y representa un día del mes, se puede ubicar en un primer período, en un segundo período o un tercer(último) período
//~ pos: según el período en el que se encuentre, se devuelve cierto valor de velocidad_viento
int velocidad_del_viento_segun_dia(int dia) {
    int velocidad_viento;
    if (es_primer_periodo(dia)) {
        velocidad_viento = VELOCIDAD_VIENTO_PRIMER_PERIODO;
    } else if (es_segundo_periodo(dia)) {
        velocidad_viento = VELOCIDAD_VIENTO_SEGUNDO_PERIODO;
    } else {
        velocidad_viento = VELOCIDAD_VIENTO_TERCER_PERIODO;
    }
    return velocidad_viento;
}

//~ pre: hora es la inicial de un momento del dia, que puede ser de mañana, tarde o noche
//~ pos: según la hora en la que se encuentre, se devolverá cierto valor humedad
int humedad_segun_hora(char hora) {
    int humedad;
    if (es_maniana(hora)) {
        humedad = HUMEDAD_MANIANA;
    } else if (es_tarde(hora)) {
        humedad = HUMEDAD_TARDE;
    } else {
        humedad = HUMEDAD_NOCHE;
    }
    return humedad;
}

//~ pre: pie indica con cuál pie se levantó LEGOLAS o GIMLI, pude ser el PIE_IZQUIERDO o PIE_DERECHO
//~ pos: según el pie con el que se levante, sumará más puntos de humor
int puntos_humor_por_pie(char pie) {
    int puntos;
    if (es_pie_derecho(pie)) {
        puntos = HUMOR_PIE_DERECHO;
    } else {
        puntos = HUMOR_PIE_IZQUIERDO;
    }
    return puntos;
}

//~ pre: cena es la inicial de una de las comidas aptas para elfos o enanos, puede ser ENSALADA, HAMBURGUESA, PIZZA o GUISO
//~ pos: segun la cena, gana distinta cantidad de puntos de humor
int puntos_humor_por_cena(char cena) {
    int puntos;
    if (cena_ensalada(cena)) {
        puntos = HUMOR_ENSALADA;
    } else if (cena_hamburguesa(cena)) {
        puntos = HUMOR_HAMBURGUESA;
    } else if (cena_pizza(cena)) {
        puntos = HUMOR_PIZZA;
    } else {
        puntos = HUMOR_GUISO;
    }
    return puntos;
}

//~ pre: horas_dormidas es un número que se encuentra entre las HORAS_MIN_POCO_DESCANSO y las HORAS_MAX de sueño
//~ pos: mientras más horas_dormidas se tengan, se van a tener más cantidad de puntos de humor
int puntos_humor_por_descanso(int horas_dormidas) {
    int puntos;
    if (es_poco_descanso(horas_dormidas)) {
        puntos = HUMOR_POCO_DESCANSO;
    } else if (es_suficiente_descanso(horas_dormidas)) {
        puntos = HUMOR_SUFICIENTE_DESCANSO;
    } else {
        puntos = HUMOR_GRAN_DESCANSO;
    }
    return puntos;
}

//~ pre: pie, cena, horas_dormidas son datos válidos ingresados por el usuario
//~ pos: según los datos recolectados, se va a poder cuantificar el humor de LEGOLAS o GIMLI
int calcular_humor(char pie, char cena, int horas_dormidas) {
    int puntaje;
    puntaje = puntos_humor_por_pie(pie);
    puntaje += puntos_humor_por_cena(cena);
    puntaje += puntos_humor_por_descanso(horas_dormidas);
    return puntaje;
}

//~ pre: puntos proviene de calcular_humor() según los datos proporcionados por el usuario respecto a uno de los líderes
//~ pos: devolverá true si tiene pocos puntos, lo que sería un humor malo
bool es_humor_malo(int puntos) {
    return (puntos >= HUMOR_MIN_MALO && puntos < HUMOR_MIN_REGULAR);
}

//~ pre: puntos proviene de calcular_humor() según los datos proporcionados por el usuario respecto a GIMLI o LEGOLAS
//~ pos: devolverá true si tiene una cantidad intermedia de puntos, garantizando un humor regular
bool es_humor_regular(int puntos) {
    return (puntos >= HUMOR_MIN_REGULAR && puntos < HUMOR_MIN_BUENO);
}

//~ pre: puntos proviene de calcular_humor() según los datos proporcionados por el usuario respecto a uno de los líderes
//~ pos: devolverá true si tiene muchos puntos, significando que tiene un humor bueno
bool es_humor_bueno(int puntos) {
    return (puntos >= HUMOR_MIN_BUENO && puntos <= HUMOR_MAX);
}

//~ pre: se ingresan datos validos de pie, cena, horas_dormidas respecto a uno de los líderes
//~ pos: devolverá la inicial del estado de ánimo que tendrán los súbitos según el humor de su líder, la cual será ANIMO_MALO, ANIMO_REGULAR o ANIMO_BUENO
char estado_de_animo(char pie, char cena, int horas_dormidas) {
    char animo;
    int puntaje;
    puntaje = calcular_humor(pie, cena, horas_dormidas);
    if (es_humor_malo(puntaje)) {
        animo = ANIMO_MALO;
    } else if (es_humor_regular(puntaje)) {
        animo = ANIMO_REGULAR;
    } else {
        animo = ANIMO_BUENO;
    }
    return animo;
}

//~ pre: viento, humedad, animo elfos, animo enanos provienen de los distintos calculos hechos
//~ pos: muestra resumen de los datos en pantalla
void mostrar_resumen(int viento, int humedad, char elfos, char enanos) {
    printf("\n\nResumen: %i %i %c %c.", viento, humedad, elfos, enanos);
}

//~ pre: viento, humedad, animo elfos, animo enanos provienen de distintos calculos
//~ pos: comunica dichos valores al usuario
void mostrar_analisis(int viento, int humedad, char elfos, char enanos) {
    printf("\nViento: %i km/h", viento);
    printf("\nHumedad: %i %%", humedad);
    printf("\nÁnimo elfos: %c", elfos);
    printf("\nÁnimo enanos: %c", enanos);
    mostrar_resumen(viento, humedad, elfos, enanos);
}

//~ pre:
//~ pos: se obtiene una fecha válida ingresada por el usuario
void preguntar_dia_del_mes(int *dia) {
    printf("Día del mes: (De 1 a 30)\nDía:");
    scanf("%i", dia);
    while (!es_dia_de_mes(*dia)) {
        printf("Eso no es un día válido... Del 1 al 30...\nDía: ");
        scanf("%i", dia);
    }
}

//~ pre:
//~ pos: se obtiene una hora del día válida ingresada por el usuario
void preguntar_hora_del_dia(char *hora) {
    printf("Hora del día: ");
    printf("[%c]Mañana [%c]Tarde [%c]Noche\nYa es de ", MANIANA, TARDE, NOCHE);
    scanf(" %c", hora);
        while (!es_hora_de_dia(*hora)) {
        printf("No entiendo qué me estas diciendo...\n");
        printf("[%c]Mañana [%c]Tarde [%c]Noche\nYa es de ", MANIANA, TARDE, NOCHE);
        scanf(" %c", hora);
    }
}

//~ pre: letra representa a LEGOLAS o GIMLI
//~ pos: se le pide al usuario ingresar un dato que corresponda al pie con el que se levantó L o G, que puede ser PIE_IZQUIERDO o PIE_DERECHO
void preguntar_pie(char *pie, char letra) {
    printf("\tPie con el que se levantó: [%c]Izquierdo [%c]Derecho \n\tPie: ", PIE_IZQUIERDO, PIE_DERECHO);
    scanf(" %c", pie);
    while (!es_pie_valido(*pie)) {
        if (es_legolas(letra)) printf("\tLegolas"); else printf("\tGrimli");
        printf(" solo tiene 2 pies. Con cuál se levantó?\n");
        printf("\t[%c]Izquierdo [%c]Derecho \n\tPie: ", PIE_IZQUIERDO, PIE_DERECHO);
        scanf(" %c", pie);
    }
}

//~ pre: letra representa a LEGOLAS o GIMLI
//~ pos: se le pide al usuario ingresar un dato que corresponda a lo que cenó L o G, que puede ser ENSALADA, HAMBURGUESA, PIZZA o GUISO
void preguntar_cena(char *cena, char letra) {
    printf("\tCena de la noche anterior: [%c]Ensalada [%c]Hamburguesa [%c]Pizza [%c]Guiso \n\tCena: ", ENSALADA, HAMBURGUESA, PIZZA, GUISO);
    scanf(" %c", cena);
    while (!es_cena_valida(*cena)) {
        if (es_legolas(letra)) printf("\tEso no es comida de elfos... Qué cenó Legolas?\n");
        else printf("\tEso no es comida de enanos... Qué cenó Gimli?\n");
        printf("\t[%c]Ensalada [%c]Hamburguesa [%c]Pizza [%c]Guiso \n\tCena: ", ENSALADA, HAMBURGUESA, PIZZA, GUISO);
        scanf(" %c", cena);
    }
}

//~ pre: letra representa a LEGOLAS o GIMLI
//~ pos: se le pide al usuario la cantidad de horas que durmió L o G, que puede ir entre HORAS_MIN_POCO_DESCANSO y HORAS_MAX horas.
void preguntar_horas_dormidas(int *horas) {
    printf("\tCantidad de horas que durmió: ");
    scanf("%i", horas);
    while (!esta_en_el_rango_de_horas(*horas)) {
        printf("\tUn lider no puede dormir esa cantidad de horas en plena guerra! Son permitidas de %i a %i! \n\tHoras: ", HORAS_MIN_POCO_DESCANSO, HORAS_MAX);
        scanf("%i", horas);
    }
}

//~ pre: letra sirve para identificar a LEGOLAS o GIMLI
//~ pos: llama a los procedimientos para que interactuen con el usuario y los pasa al programa
void mostrar_preguntas_lider(char *pie, char *cena, int *horas_dormidas, char letra) {
    if (es_legolas(letra)) printf("\nLegolas:\n"); else printf("\nGimli:\n");
    preguntar_pie(pie, letra);
    preguntar_cena(cena, letra);
    preguntar_horas_dormidas(horas_dormidas);
}

void animos(int* viento , int* humedad , char* animo_legolas , char* animo_gimli) {
    int dia;
    char hora;
    char legolas_pie;
    char legolas_cena;
    int legolas_horas_dormidas;
    char gimli_pie;
    char gimli_cena;
    int gimli_horas_dormidas;

    system("clear");
    printf("Reporte de batalla\n\n");

    preguntar_dia_del_mes(&dia);
    preguntar_hora_del_dia(&hora);
    mostrar_preguntas_lider(&legolas_pie, &legolas_cena, &legolas_horas_dormidas, LEGOLAS);
    mostrar_preguntas_lider(&gimli_pie, &gimli_cena, &gimli_horas_dormidas, GIMLI);

    *viento = velocidad_del_viento_segun_dia(dia);
    *humedad = humedad_segun_hora(hora);
    *animo_legolas = estado_de_animo(legolas_pie, legolas_cena, legolas_horas_dormidas);
    *animo_gimli = estado_de_animo(gimli_pie, gimli_cena, gimli_horas_dormidas);

    mostrar_analisis(*viento, *humedad, *animo_legolas, *animo_gimli);
}
