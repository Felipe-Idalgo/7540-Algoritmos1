#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARCHIVO 100

const int ERROR = -1;
const int COMANDO = 1;
const char RANKING[] = "ranking";
const char CREAR_CAMINOS[] = "crear_camino";
const char CREAR_CONFIGURACION[] = "crear_configuracion";
const char VER_REPETICION[] = "poneme_la_repe";
const char JUGAR_PARTIDA[] = "jugar";

const int ARGUMENTO = 2;
const char LISTAR[] = "listar=";
const char CONFIGURACION[] = "config=";
const char GRABACION[] = "grabacion=";
const char VELOCIDAD[] = "velocidad=";
const char SIN_ARCHIVO[] = "";
const int LIMITE_ARGUMENTOS_RANKING = 4;
const int LIMITE_ARGUMENTOS_CREAR_CAMINOS = 3;
const int LIMITE_ARGUMENTOS_CREAR_CONFIGURACION = 3;
const int LIMITE_ARGUMENTOS_VER_REPETICION = 4;
const int LIMITE_ARGUMENTOS_JUGAR_PARTIDA = 4;



//~ Pre: Recibe la cantidad de argumentos que se ingresaron desde la línea de comandos.
//~ Pos: Devuelve true cuando hay más de uno, suponiendo que se ha ingresado un comando.
bool ingresa_comando(int argc) {
    return argc > COMANDO;
}


//~ Pre: Recibe la cantidad de argumentos que se ingresaron desde la línea de comandos.
//~ Pos: Devuelve true cuando más de dos, suponiendo que se ha ingresado un argumento después de un comando.
bool ingresa_argumento(int argc) {
    return argc > ARGUMENTO;
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a RANKING
bool ingresa_ranking(char comando[]) {
    return !strcmp(comando, RANKING);
}



//~ Pre: Recibe los argumentos de la línea de comando junto a su tope. *cant_a_listar no está inicializado.
//~ Pos: Devuelve true si se ha ingresado LISTAR e inicializa *cant_a_listar con el valor numérico siguiente a LISTAR.
//~      *cant_a_listar tomará valores numéricos hasta encontrar uno no numérico. No comprueba la calidad del dato ingresado.
bool ingresa_listar(int argc, char *argv[], int *cant_a_listar) {
    bool hay_que_listar = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_que_listar) {
        if (!strncmp(argv[i], LISTAR, strlen(LISTAR))) {
            hay_que_listar = true;
            size_t pos_numero = strlen(LISTAR);
            *cant_a_listar = atoi(&(argv[i][pos_numero]));
        }
        i++;
    }
    return hay_que_listar;
}



//~ Pre: Recibe los argumentos de la línea de comandos. archivo se encuentra sin inicializar.
//~ Pos: Devuelve true si se ha ingresado CONFIGURACION e iniciliza archivo con la cadena siguiente a CONFIGURACION.
//~      No se comprueba si archivo es un archivo de configuración válido (si existe, si se tienen permisos de lectura, ni si es .txt).
bool ingresa_configuracion(int argc, char *argv[], char archivo[MAX_ARCHIVO]) {
    bool hay_configuracion = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_configuracion) {
        if (!strncmp(argv[i], CONFIGURACION, strlen(CONFIGURACION))) {
            hay_configuracion = true;
            size_t pos_configuracion = strlen(CONFIGURACION);
            strcpy(archivo, &(argv[i][pos_configuracion]));
        }
        i++;
    }
    return hay_configuracion;
}



//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de RANKING válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene el nombre del archivo de configuracion y la cantidad de puntajes a listar.
void comprobar_ranking(int argc, char *argv[], char config[MAX_ARCHIVO], int *cantidad, bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_RANKING) {
        printf("Mucho argumento\n");
        *es_valido = false;
        return;
    }

    if (ingresa_listar(argc, argv, cantidad)) {
        if (!*cantidad) {
            printf("No se ingresó bien una cantidad a listar.\n");
            *es_valido = false;
        }
        printf("Listar: %i\n", *cantidad);
    }
    if (ingresa_configuracion(argc, argv, config)) {
        if (!strcmp(config, SIN_ARCHIVO)) {
            printf("No se ingresó bien la configuración\n");
            *es_valido = false;
        }
        printf("Configuracion: %s\n", config);
    }
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a CREAR_CAMINOS
bool ingresa_crear_caminos(char comando[]) {
    return !strcmp(comando, CREAR_CAMINOS);
}



//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de CREAR_CAMINOS válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de caminos a crear o sobreescribir.
void comprobar_crear_caminos(int argc, char *argv[], char archivo[MAX_ARCHIVO], bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_CREAR_CAMINOS) {
        printf("Mucho argumento\n");
        *es_valido = false;
    } else if (!ingresa_argumento(argc)) {
        printf("Falta nombre de archivo\n");
        *es_valido = false;
    } else {
        strcpy(archivo, argv[ARGUMENTO]);
        printf("Archivo: %s\n", archivo);
    }
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a CREAR_CONFIGURACION
bool ingresa_crear_configuracion(char comando[]) {
    return !strcmp(comando, CREAR_CONFIGURACION);
}



//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de CREAR_CONFIGURACIÓN válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de configuración a crear o sobreescribir.
void comprobar_crear_configuracion(int argc, char *argv[], char archivo[MAX_ARCHIVO], bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_CREAR_CONFIGURACION) {
        printf("Mucho argumento\n");
        *es_valido = false;
    } else if (!ingresa_argumento(argc)) {
        printf("Falta nombre de archivo.\n");
        *es_valido = false;
    } else {
        strcpy(archivo, argv[ARGUMENTO]);
        printf("Archivo: %s\n", archivo);
    }
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a VER_REPETICION
bool ingresa_ver_repeticion(char comando[]) {
    return !strcmp(comando, VER_REPETICION);
}



//~ Pre: Recibe los argumentos de la línea de comandos. archivo se encuentra sin inicializar.
//~ Pos: Devuelve true si se ha ingresado GRABACION e iniciliza archivo con la cadena siguiente a GRABACION.
//~      No se comprueba si archivo es un archivo de grabacion válido (si existe, si se tienen permisos de lectura, ni si es .dat).
bool ingresa_grabacion(int argc, char *argv[], char archivo[MAX_ARCHIVO]) {
    bool hay_grabacion = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_grabacion) {
        if (!strncmp(argv[i], GRABACION, strlen(GRABACION))) {
            hay_grabacion = true;
            size_t pos_grabacion = strlen(GRABACION);
            strcpy(archivo, &(argv[i][pos_grabacion]));
        }
        i++;
    }
    return hay_grabacion;
}



//~ Pre: Recibe los argumentos de la línea de comando junto a su tope. *velocidad no está inicializado.
//~ Pos: Devuelve true si se ha ingresado VELOCIDAD e inicializa *velocidad con el valor numérico siguiente a LISTAR.
//~      *velocidad tomará valores numéricos hasta encontrar uno no numérico. No comprueba la calidad del dato ingresado.
bool ingresa_velocidad(int argc, char *argv[], float *velocidad) {
    bool hay_velocidad = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_velocidad) {
        if (!strncmp(argv[i], VELOCIDAD, strlen(VELOCIDAD))) {
            hay_velocidad = true;
            size_t pos_velocidad = strlen(VELOCIDAD);
            *velocidad = atof(&(argv[i][pos_velocidad]));
        }
        i++;
    }
    return hay_velocidad;
}



//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de VER_REPETICION válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de grabacion y la velocidad a reproducir.
void comprobar_ver_repeticion(int argc, char *argv[], char grabacion[MAX_ARCHIVO], float *velocidad, bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_VER_REPETICION) {
        printf("Mucho argumento\n");
        *es_valido = false;
    } else if (
        !ingresa_argumento(argc)
        || !ingresa_grabacion(argc, argv, grabacion)
        || !strcmp(grabacion, SIN_ARCHIVO)
    ) {
        printf("Falta grabación\n");
        *es_valido = false;
    } else {
        printf("Grabacion: %s\n", grabacion);
        if (ingresa_velocidad(argc, argv, velocidad) && *velocidad) {
            printf("Velocidad: %f\n", *velocidad);
        } else {
            printf("Velocidad default\n");
        }
    }
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a JUGAR_PARTIDA
bool ingresa_jugar_partida(char comando[]) {
    return !strcmp(comando, JUGAR_PARTIDA);
}



//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de JUGAR_PARTIDA válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de configuración y de grabación.
void comprobar_jugar_partida(int argc, char *argv[], char config[MAX_ARCHIVO], char grabacion[MAX_ARCHIVO], bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_JUGAR_PARTIDA) {
        printf("Mucho argumento\n");
    } else {
        if (ingresa_configuracion(argc, argv, config) && strcmp(config, SIN_ARCHIVO)) {
            printf("Configuracion: %s\n", config);
        } else {
            printf("Configuracion default\n");
        }
        if (ingresa_grabacion(argc, argv, grabacion) && strcmp(grabacion, SIN_ARCHIVO)) {
            printf("Grabacion: %s\n", grabacion);
        }
    }
}



int main(int argc, char *argv[]) {
    if (!ingresa_comando(argc)) {
        printf("No puedo hacer nada si no me decis qué querés.\n");
        return ERROR;
    }

    bool es_valido = true;
    char config[MAX_ARCHIVO], caminos[MAX_ARCHIVO], grabacion[MAX_ARCHIVO];
    int cant_a_listar;
    float velocidad;

    if (ingresa_ranking(argv[COMANDO])) {
        /*
         * ./defendiendo ranking listar=num config=config
         * listar y config opcionales.
         * listar default: mostrar todos de una configuracion
         */
        printf("%s\n", RANKING);
        comprobar_ranking(argc, argv, config, &cant_a_listar, &es_valido);
        //~ if (es_valido) mostrar_ranking(cant_a_listar, config);
    } else if (ingresa_crear_caminos(argv[COMANDO])) {
        /*
         * ./defendiendo crear_camino mis_caminos.txt
         * archivo obligatorio
         * mostrar en pantalla como va quedado
         */
        printf("%s\n", CREAR_CAMINOS);
        comprobar_crear_caminos(argc, argv, caminos, &es_valido);
        //~ if (es_valido) crear_caminos(caminos);
    } else if (ingresa_crear_configuracion(argv[COMANDO])) {
        /*
         * ./defendiendo crear_configuracion mi_configuracion.txt
         * archivo obligatorio
         * caracteristicas predeterminadas: -1
         * configurar RESISTENCIA_TORRES
         * configurar ENANOS_INICIO
         * configurar ELFOS_INICIO
         * configurar ENANOS_EXTRA
         * configurar ELFOS_EXTRA
         * configurar ENANOS_ANIMO
         * configurar ELFOS_ANIMO
         * configurar VELOCIDAD del juego
         * configurar CAMINOS = caminos.txt
         */
        printf("%s\n", CREAR_CONFIGURACION);
        comprobar_crear_configuracion(argc, argv, config, &es_valido);
        //~ if (es_valido) crear_configuracion(config);
    } else if (ingresa_ver_repeticion(argv[COMANDO])) {
        /*
         * ./defendiendo poneme_la_repe grabacion=mi_partida.dat velocidad=0.5
         * grabacion obligatoria, velocidad opcional
         * velocidad solicitada o 1 (default)
         */
        printf("%s\n", VER_REPETICION);
        comprobar_ver_repeticion(argc, argv, grabacion, &velocidad, &es_valido);
        //~ if (es_valido) ver_repeticion(grabacion, velocidad);
    } else if (ingresa_jugar_partida(argv[COMANDO])) {
        /*
         * ./defendiendo jugar config=mi_configuracion.txt grabacion=mi_partida.dat
         * si el archivo no existe o no fue ingresado: config_default
         * grabacion: (sobre)escribe si fue ingresado, si no no se graba
         */
        printf("%s\n", JUGAR_PARTIDA);
        comprobar_jugar_partida(argc, argv, config, grabacion, &es_valido);
    } else {
        printf("No se ingreso ningun comando correcto.\n");
    }

    printf("\n");
    return 0;
}
 
