#include "defendiendo_torres.h"
#include "configuracion.h"
#include "caminos.h"
#include "juego.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define ERROR -1

const int   PROGRAMA = 0, COMANDO = 1, ARGUMENTO = 2;

const char  RANKING[] = "ranking",
            CREAR_CAMINOS[] = "crear_camino",
            CREAR_CONFIGURACION[] = "crear_configuracion",
            VER_REPETICION[] = "poneme_la_repe",
            JUGAR_PARTIDA[] = "jugar";

const char  LISTAR[] = "listar=", CONFIGURACION[] = "config=",
            GRABACION[] = "grabacion=", VELOCIDAD[] = "velocidad=";

const char  SEPARACION[] = "_", CARACTER_FINAL = '\0';

const char  EXTENSION_RANKING[] = ".csv",
            EXTENSION_CAMINOS[] = ".txt",
            EXTENSION_CONFIGURACION[] = ".txt",
            EXTENSION_GRABACION[] = ".dat";

const char  ARCHIVO_RANKING[] = "ranking",
            ARCHIVO_RANKING_AUX[] = "tmp",
            FORMATO_LECTURA_RANKING[] = "%[^;];%i\n",
            FORMATO_ESCRITURA_RANKING[] = "%s;%i\n",
            FORMATO_ENCABEZADO_RANKING[] = " G A N A D O R E S\n",
                             SUBRAYADO[] = "~~~~~~~~~~~~~~~~~~~~~~\n",
            FORMATO_IMPRESION_RANKING[] =  "%i. %s, con %i ptos.\n";

const int   LIMITE_ARGUMENTOS_RANKING = 4, LIMITE_ARGUMENTOS_CREAR_CAMINOS = 3,
            LIMITE_ARGUMENTOS_CREAR_CONFIGURACION = 3, LIMITE_ARGUMENTOS_VER_REPETICION = 4,
            LIMITE_ARGUMENTOS_JUGAR_PARTIDA = 4;

/*
 * Muestra los comandos en pantalla para informarle al usuario las posibles opciones.
 */
int mostrar_comandos(char programa[]) {
    printf("Comandos:\n");
    printf("\n%s %s %s25 %smi_configuracion%s\n", programa, RANKING, LISTAR, CONFIGURACION, EXTENSION_CONFIGURACION);
    printf("> Se muestra el ranking para la configuración dada y se muestra el top de la cantidad indicada.\n");
    printf("> En caso de no recibir configuración y/o lista, se toman los valores por defecto.\n");
    printf("\n%s %s mi_camino%s\n", programa, CREAR_CAMINOS, EXTENSION_CAMINOS);
    printf("> Se permite crear caminos personalizados. Se guardarán en el archivo dado por eso es obligatorio.\n");
    printf("> Si el archivo no existe, se creará, si existe se sobreescribirá.\n");
    printf("\n%s %s mi_configuracion%s\n", programa, CREAR_CONFIGURACION, EXTENSION_CONFIGURACION);
    printf("> Se permite crear configuraciones personalizadas. Se guardarán en el archivo dado por eso es obligatorio.\n");
    printf("> Si el archivo no existe, se creará, si existe se sobreescribirá.\n");
    printf("\n%s %s %smi_grabacion%s %s0.5\n", programa, VER_REPETICION, GRABACION, EXTENSION_GRABACION, VELOCIDAD);
    printf("> Para ver la repetición de un juego es necesario indicar la grabación. La velocidad de reproducción es opcional\n");
    printf("\n%s %s %smi_configuracion%s %smi_grabacion%s\n", programa, JUGAR_PARTIDA, CONFIGURACION, EXTENSION_CONFIGURACION, GRABACION, EXTENSION_GRABACION);
    printf("> Se pueden jugar configuraciones personalizadas. Para grabar la partida tiene que crear o sobreescribir una existente\n");
    printf("> Ambos parámetros son opcionales\n");
    return !ERROR;
}

/*
 * Pre: Recibe la cantidad de argumentos que se ingresaron desde la línea de comandos.
 * Pos: Devuelve true cuando hay más de uno, y se supone que hay un comando.
 */
bool ingresa_comando(int argc) {
    return argc > COMANDO;
}

/*
 * Pre: Recibe la cantidad de argumentos que se ingresaron desde la línea de comandos.
 * Pos: Devuelve true cuando hay más de dos, suponiendo que se ha ingresado un argumento después de un comando.
 */
bool ingresa_argumento(int argc) {
    return argc > ARGUMENTO;
}

/*
 * Pre: Recibe un argumento y la etiqueta que se desee saber si tiene.
 * Pos: Devuelve true cuando el argumento tiene la etiqueta indicada.
 */
bool ingresa_etiqueta(char argumento[], const char etiqueta[]) {
    return !strncmp(argumento, etiqueta, strlen(etiqueta));
}

/*
 * Pre: Recibe el nombre de un archivo y la extensión que se espera encontrar.
 * Pos: Devuelve true si tiene dicha extensión.
 */
bool tiene_extension_valida(char archivo[MAX_ARCHIVO], const char extension[]) {
    bool es_valida = false;
    char* ext_archivo = strrchr(archivo, (int) *extension);
    if (ext_archivo != NULL) {
        if (!strcmp(ext_archivo, extension)) {
            es_valida = true;
        }
    }
    return es_valida;
}

/*
 * Pre: Recibe el nombre de un archivo.
 * Pos: Devuelve true si no se ha dejado el campo vacío.
 */
bool ingresa_archivo(char archivo[MAX_ARCHIVO]) {
    return strcmp(archivo, SIN_ARCHIVO);
}

/*
 * Pre: Recibe el argumento que esté en la posición de COMANDO.
 * Pos: Devuelve true si el argumento corresponde a RANKING
 */
bool ingresa_ranking(char comando[]) {
    return !strcmp(comando, RANKING);
}

/*
 * Pre: Recibe el argumento que esté en la posición de COMANDO.
 * Pos: Devuelve true si el argumento corresponde a CREAR_CAMINOS
 */
bool ingresa_crear_caminos(char comando[]) {
    return !strcmp(comando, CREAR_CAMINOS);
}

/*
 * Pre: Recibe el argumento que esté en la posición de COMANDO.
 * Pos: Devuelve true si el argumento corresponde a CREAR_CONFIGURACION
 */
bool ingresa_crear_configuracion(char comando[]) {
    return !strcmp(comando, CREAR_CONFIGURACION);
}

/*
 * Pre: Recibe el argumento que esté en la posición de COMANDO.
 * Pos: Devuelve true si el argumento corresponde a VER_REPETICION
 */
bool ingresa_ver_repeticion(char comando[]) {
    return !strcmp(comando, VER_REPETICION);
}

/*
 * Pre: Recibe el argumento que esté en la posición de COMANDO.
 * Pos: Devuelve true si el argumento corresponde a JUGAR_PARTIDA
 */
bool ingresa_jugar_partida(char comando[]) {
    return !strcmp(comando, JUGAR_PARTIDA);
}

/*
 * Pre: Recibe los argumentos de la línea de comando junto a su tope.
 *      *cant_a_listar es la variable donde se almacenará la cantidad a listar.
 * Pos: Devuelve true si se ha ingresado LISTAR e inicializa *cant_a_listar con
 *      el valor numérico siguiente a LISTAR.
 */
bool ingresa_listar(int argc, char *argv[], int *cant_a_listar) {
    bool hay_que_listar = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_que_listar) {
        if (ingresa_etiqueta(argv[i], LISTAR)) {
            hay_que_listar = true;
            size_t pos_numero = strlen(LISTAR);
            *cant_a_listar = atoi(&(argv[i][pos_numero]));
        }
        i++;
    }
    return hay_que_listar;
}

/*
 * Pre: Recibe los argumentos de la línea de comandos. archivo se encuentra sin inicializar.
 * Pos: Devuelve true si se ha ingresado CONFIGURACION e iniciliza archivo con la
 *      cadena siguiente a CONFIGURACION. No se comprueba si la ruta puede abrirse.
 */
bool ingresa_configuracion(int argc, char *argv[], char archivo[MAX_ARCHIVO]) {
    bool hay_configuracion = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_configuracion) {
        if (ingresa_etiqueta(argv[i], CONFIGURACION)) {
            hay_configuracion = true;
            size_t pos_configuracion = strlen(CONFIGURACION);
            strcpy(archivo, &(argv[i][pos_configuracion]));
        }
        i++;
    }
    return hay_configuracion;
}

/*
 * Pre: Recibe los argumentos de la línea de comandos. archivo se encuentra sin inicializar.
 * Pos: Devuelve true si se ha ingresado GRABACION e iniciliza archivo con la cadena
 *      siguiente a GRABACION. No se comprueba si la ruta puede abrirse.
 */
bool ingresa_grabacion(int argc, char *argv[], char archivo[MAX_ARCHIVO]) {
    bool hay_grabacion = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_grabacion) {
        if (ingresa_etiqueta(argv[i], GRABACION)) {
            hay_grabacion = true;
            size_t pos_grabacion = strlen(GRABACION);
            strcpy(archivo, &(argv[i][pos_grabacion]));
        }
        i++;
    }
    return hay_grabacion;
}

/*
 * Pre: Recibe los argumentos de la línea de comando junto a su tope. *velocidad no está inicializado.
 * Pos: Devuelve true si se ha ingresado VELOCIDAD e inicializa *velocidad con el valor numérico
 *      siguiente a LISTAR.
 */
bool ingresa_velocidad(int argc, char *argv[], float *velocidad) {
    bool hay_velocidad = false;
    int i = ARGUMENTO;
    while (i < argc && !hay_velocidad) {
        if (ingresa_etiqueta(argv[i], VELOCIDAD)) {
            hay_velocidad = true;
            size_t pos_velocidad = strlen(VELOCIDAD);
            *velocidad = (float) atof(&(argv[i][pos_velocidad]));
        }
        i++;
    }
    return hay_velocidad;
}

/*
 * Pre: Recibe el nombre de un archivo y la extensión.
 * Pos: Quita la extensión del nombre del archivo.
 */
void quitar_extension(char archivo[MAX_ARCHIVO], const char extension[]) {
    char* ext_archivo = strrchr(archivo, (int) *extension);
    if (ext_archivo != NULL)
        *ext_archivo = FIN_CADENA;
}

/*
 * Pre: Recibe todos los argumentos de la línea de comandos junto a su tope.
 *      Se presupone que son argumentos de RANKING válidos.
 * Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error,
 *      se imprime un mensaje en pantalla y *es_valido cambia su valor a falso. Se obtiene
 *      el nombre del archivo de configuracion y la cantidad de puntajes a listar.
 */
void comprobar_ranking(int argc, char *argv[], char config[MAX_ARCHIVO], int *cantidad, bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_RANKING) {
        printf("Mucho argumento\n");
        *es_valido = false;
        return;
    }
    if (ingresa_listar(argc, argv, cantidad)) {
        if (!*cantidad) {
            printf("No se ingresó bien la cantidad a listar.\n");
            *es_valido = false;
        }
    } else *cantidad = INDEFINIDO;

    if (ingresa_configuracion(argc, argv, config)) {
        if (!tiene_extension_valida(config, EXTENSION_CONFIGURACION)) {
            quitar_extension(config, EXTENSION);
            printf("El archivo no tiene una extensión válida. Tiene que llamarse %s%s\n", config, EXTENSION_CONFIGURACION);
            *es_valido = false;
        }
    } else strcpy(config, SIN_ARCHIVO);
}

/*
 * Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone
 *      que son argumentos de CREAR_CAMINOS válidos.
 * Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra
 *      un error, se imprime un mensaje en pantalla y *es_valido cambia su valor
 *      a falso. Se obtiene la ruta del archivo de caminos a crear o sobreescribir.
 */
void comprobar_crear_caminos(int argc, char *argv[], char archivo[MAX_ARCHIVO], bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_CREAR_CAMINOS) {
        printf("Mucho argumento.\n");
        *es_valido = false;
    } else if (!ingresa_argumento(argc)) {
        printf("Falta ingresar el nombre de archivo%s.\n", EXTENSION_CAMINOS);
        *es_valido = false;
    } else {
        strcpy(archivo, argv[ARGUMENTO]);
        if (!tiene_extension_valida(archivo, EXTENSION_CAMINOS)) {
            quitar_extension(archivo, EXTENSION);
            printf("La extensión del archivo no es válida. Tiene que llamarse %s%s.\n", archivo, EXTENSION_CAMINOS);
            *es_valido = false;
        }
    }
}

/*
 * Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone
 *      que son argumentos de CREAR_CONFIGURACIÓN válidos.
 * Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error,
 *      se imprime un mensaje en pantalla y *es_valido cambia su valor a falso.
 *      Se obtiene la ruta del archivo de configuración a crear o sobreescribir.
 */
void comprobar_crear_configuracion(int argc, char *argv[], char archivo[MAX_ARCHIVO], bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_CREAR_CONFIGURACION) {
        printf("Mucho argumento\n");
        *es_valido = false;
    } else if (!ingresa_argumento(argc)) {
        printf("Falta ingresar el nombre de archivo%s.\n", EXTENSION_CONFIGURACION);
        *es_valido = false;
    } else {
        strcpy(archivo, argv[ARGUMENTO]);
        if (!tiene_extension_valida(archivo, EXTENSION_CONFIGURACION)) {
            quitar_extension(archivo, EXTENSION);
            printf("La extensión del archivo no es válida. Tiene que ser %s%s\n", archivo, EXTENSION_CONFIGURACION);
            *es_valido = false;
        }
    }
}

/*
 * Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone
 *      que son argumentos de VER_REPETICION válidos.
 * Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error,
 *      se imprime un mensaje en pantalla y *es_valido cambia su valor a falso.
 *      Se obtiene la ruta del archivo de grabacion y la velocidad a reproducir.
 */
void comprobar_ver_repeticion(int argc, char *argv[], char grabacion[MAX_ARCHIVO], float *velocidad, bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_VER_REPETICION) {
        printf("Mucho argumento\n");
        *es_valido = false;
    } else if (!ingresa_argumento(argc) || !ingresa_grabacion(argc, argv, grabacion)) {
        printf("Tiene que ingresar %smi_grabacion%s\n", GRABACION, EXTENSION_GRABACION);
        *es_valido = false;
    }  else if (!tiene_extension_valida(grabacion, EXTENSION_GRABACION)) {
        quitar_extension(grabacion, EXTENSION);
        printf("La grabación no tiene una extensión válida. Tiene que llamarse %s%s\n", grabacion, EXTENSION_GRABACION);
        *es_valido = false;
    } else {
        if (!ingresa_velocidad(argc, argv, velocidad)) {
            *velocidad = (float) INDEFINIDO;
        } else if (!*velocidad) {
            printf("Parece que la velocidad que ingresaste no es válida.\n");
        }
    }
}

/*
 * Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone
 *      que son argumentos de JUGAR_PARTIDA válidos.
 * Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error,
 *      se imprime un mensaje en pantalla y *es_valido cambia su valor a falso.
 *      Se obtiene nombre del archivo de configuración y de grabación.
 */
void comprobar_jugar_partida(int argc, char *argv[], char config[MAX_ARCHIVO], char grabacion[MAX_ARCHIVO], bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_JUGAR_PARTIDA) {
        printf("Mucho argumento\n");
        *es_valido = false;
    } else {
        if (ingresa_configuracion(argc, argv, config)) {
            if (!tiene_extension_valida(config, EXTENSION_CONFIGURACION)) {
                quitar_extension(config, EXTENSION);
                printf("La configuración no está con la extensión adecuada. Tiene que ser %s%s.\n", config, EXTENSION_CONFIGURACION);
                *es_valido = false;
            }
        } else strcpy(config, SIN_ARCHIVO);

        if (ingresa_grabacion(argc, argv, grabacion)) {
            if (!tiene_extension_valida(grabacion, EXTENSION_GRABACION)) {
                quitar_extension(grabacion, EXTENSION);
                printf("La grabación no está con la extensión adecuada. Tiene que ser %s%s\n", grabacion, EXTENSION_GRABACION);
                *es_valido = false;
            }
        } else strcpy(grabacion, SIN_ARCHIVO);
    }
}

/*
 * Devuelve true si archivo pudo ser inicializado. Si no, muestra en
 * pantalla que no se pudo abrir el archivo.
 */
bool se_puede_abrir(FILE* archivo, const char nombre[MAX_ARCHIVO]) {
    if(!archivo) {
        printf("No se pudo abrir %s, campeón\n", nombre);
    }
    return archivo;
}

/*
 * Pre: Recibe el nombre de un archivo de configuración.
 * Pos: Inicializa ranking con el nombre del archivo de una configuración personalizada o predeterminada.
 */
void obtener_ranking(char ranking[MAX_ARCHIVO], char config[MAX_ARCHIVO]) {
    strcpy(ranking, ARCHIVO_RANKING);
    if (ingresa_archivo(config)) {
        quitar_extension(config, EXTENSION_CONFIGURACION);
        strcat(ranking, SEPARACION);
        strcat(ranking, config);
    }
    strcat(ranking, EXTENSION_RANKING);
}

/*
 * Pre: Recibe el nombre de una configuración, o ninguna si no se ha elegido, y el rank
 *      de una partida, con nombre y puntaje.
 * Pos: Inserta el rank en un archivo (si no existe lo crea) de manera ordenada
 *      por puntaje descendiente.
 *      Devuelve ERROR si no puede actualizarse el ranking.
 */
int grabar_rank(char config[MAX_ARCHIVO], rank_t rank) {
    FILE *archivo, *archivo_nuevo;
    char ranking[MAX_ARCHIVO];
    int leidos;
    rank_t rank_aux;
    obtener_ranking(ranking, config);
    archivo_nuevo = fopen(ARCHIVO_RANKING_AUX, ESCRITURA);
    if (!archivo_nuevo) {
        printf("No se puede actualizar el ranking\n");
        return ERROR;
    }
    archivo = fopen(ranking, LECTURA);
    if (archivo) {
        leidos = fscanf(archivo, FORMATO_LECTURA_RANKING, rank_aux.nombre, &(rank_aux.puntaje));
        while (leidos != EOF) {
            if (rank.puntaje > rank_aux.puntaje) {
                fprintf(archivo_nuevo, FORMATO_ESCRITURA_RANKING, rank.nombre, rank.puntaje);
                rank = rank_aux;
            } else {
                fprintf(archivo_nuevo, FORMATO_ESCRITURA_RANKING, rank_aux.nombre, rank_aux.puntaje);
            }
            leidos = fscanf(archivo, FORMATO_LECTURA_RANKING, rank_aux.nombre, &(rank_aux.puntaje));
        }
        fclose(archivo);
    }
    fprintf(archivo_nuevo, FORMATO_ESCRITURA_RANKING, rank.nombre, rank.puntaje);
    fclose(archivo_nuevo);
    rename(ARCHIVO_RANKING_AUX, ranking);
    return !ERROR;
}

/*
 * Pre: Puede recibir la cantidad de puntajes a mostrar y el nombre de una configuración.
 * Pos: Mostrará en pantalla el ranking de la configuración ingresada o de la de predeterminada.
 *      Mostrará la cantidad de puntajes pedida o todas si cant_a_listar es INDEFINIDO.
 *      Devuelve ERROR si no puede mostrarse.
 */
int ejecutar_ranking(int cant_a_listar, char config[MAX_ARCHIVO]) {
    int i = 1, leidos;
    char ranking[MAX_ARCHIVO];
    rank_t rank;
    obtener_ranking(ranking, config);
    FILE* archivo = fopen(ranking, LECTURA);
    if (!se_puede_abrir(archivo, ranking)) {
        printf("Tal vez tengas que jugar unas veces para generar un ranking.\n");
        return ERROR;
    }
    printf(FORMATO_ENCABEZADO_RANKING);
    printf(SUBRAYADO);
    leidos = fscanf(archivo, FORMATO_LECTURA_RANKING, rank.nombre, &rank.puntaje);
    while (leidos != EOF && (i <= cant_a_listar || cant_a_listar == INDEFINIDO)) {
        printf(FORMATO_IMPRESION_RANKING, i, rank.nombre, rank.puntaje);
        leidos = fscanf(archivo, FORMATO_LECTURA_RANKING, rank.nombre, &rank.puntaje);
        i++;
    }
    fclose(archivo);
    return !ERROR;
}

/*
 * Pre: Recibe la ruta del archivo donde van a guardarse los nuevos caminos.
 * Pos: Permite al usuario que cree caminos. Se guardarán en un archivo que
 *      creará o sobreescribirá.
 *      Devuelve ERROR si no puede crearse el archivo.
 */
int  ejecutar_crear_caminos(char caminos[MAX_ARCHIVO]) {
    return crear_nuevos_caminos(caminos);
}

/*
 * Pre: Recibe la ruta de un archivo donde va a guardarse la nueva configuración.
 * Pos: Permite al usaurio elegir las configuraciones que quiere.
 *      Devuelve ERROR si no puede guardarse la configuración.
 */
int ejecutar_crear_configuracion(char config[MAX_ARCHIVO]) {
    return crear_nueva_configuracion(config);
}

/*
 * Pre: Recibe la ruta de un archivo de grabación y el tiempo de demora entre turnos.
 * Pos: Se mostrará en pantalla la repetición de la grabación.
 *      Devuelve ERROR si no se puede abrir el archivo.
 */
int ejecutar_ver_repeticion(char grabacion[MAX_ARCHIVO], float velocidad) {
    return reproducir_juego(grabacion, velocidad);
}

/*
 * Pre: Puede ingresar la ruta de un archivo de configuración y el nombre
 *      de un archivo de grabación para crear.
 * Pos: Se podrá ejecutar el juego con una configuración personalizada y grabar el juego.
 *      Devolverá 0 si no ocurre ningún error.
 */
int ejecutar_jugar_partida(char config[MAX_ARCHIVO], char grabacion[MAX_ARCHIVO]) {
    configuracion_t configuracion;
    rank_t rank;
    return (
        (cargar_configuracion(&configuracion, config) == ERROR)
        || (iniciar_juego(configuracion, grabacion, &rank) == ERROR)
        || (grabar_rank(config, rank) == ERROR)
        || (ejecutar_ranking(INDEFINIDO, config) == ERROR)
    );
}

/*
 * Pre: Recibe los argumentos de la línea de comandos y la cantidad que hay.
 * Pos: Ejecuta una acción y devuelve 0 si la acción pudo completarse sin
 *      problemas.
 */
int comprobar_comandos(int argc, char *argv[]) {
    bool es_valido = true;
    char    config[MAX_ARCHIVO] = {'\0'},
            caminos[MAX_ARCHIVO] = {'\0'},
            grabacion[MAX_ARCHIVO] = {'\0'};
    int cant_a_listar = INDEFINIDO; float velocidad = INDEFINIDO;

    if (ingresa_ranking(argv[COMANDO])) {
        comprobar_ranking(argc, argv, config, &cant_a_listar, &es_valido);
        if (es_valido) ejecutar_ranking(cant_a_listar, config);

    } else if (ingresa_crear_caminos(argv[COMANDO])) {
        comprobar_crear_caminos(argc, argv, caminos, &es_valido);
        if (es_valido) ejecutar_crear_caminos(caminos);

    } else if (ingresa_crear_configuracion(argv[COMANDO])) {
        comprobar_crear_configuracion(argc, argv, config, &es_valido);
        if (es_valido) ejecutar_crear_configuracion(config);

    } else if (ingresa_ver_repeticion(argv[COMANDO])) {
        comprobar_ver_repeticion(argc, argv, grabacion, &velocidad, &es_valido);
        if (es_valido) ejecutar_ver_repeticion(grabacion, velocidad);

    } else if (ingresa_jugar_partida(argv[COMANDO])) {
        comprobar_jugar_partida(argc, argv, config, grabacion, &es_valido);
        if (es_valido) ejecutar_jugar_partida(config, grabacion);

    } else {
        printf("No se ingresó ningún comando correcto.\n");
    }
    return es_valido;
}

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));

    if (!ingresa_comando(argc))
        return mostrar_comandos(argv[PROGRAMA]);

    return comprobar_comandos(argc, argv);
}
