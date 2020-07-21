#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "defendiendo_torres.h"
#include "juego.h"



const int   PROGRAMA = 0, COMANDO = 1, ARGUMENTO = 2;

const char  RANKING[] = "ranking", CREAR_CAMINOS[] = "crear_camino",
            CREAR_CONFIGURACION[] = "crear_configuracion", VER_REPETICION[] = "poneme_la_repe",
            JUGAR_PARTIDA[] = "jugar";

const char  LISTAR[] = "listar=", CONFIGURACION[] = "config=",
            GRABACION[] = "grabacion=", VELOCIDAD[] = "velocidad=";

const char  SIN_ARCHIVO[] = "", SEPARACION[] = "_", CARACTER_FINAL = '\0';

const char  EXTENSION[] = ".", EXTENSION_RANKING[] = ".csv", EXTENSION_CAMINOS[] = ".txt",
            EXTENSION_CONFIGURACION[] = ".txt", EXTENSION_GRABACION[] = ".dat";

const char  ARCHIVO_RANKING[] = "ranking",
            ARCHIVO_RANKING_AUX[] = "tmp.csv",
            FORMATO_LECTURA_RANKING[] = "%[^;];%i\n",
            FORMATO_ESCRITURA_RANKING[] = "%s;%i\n",
            FORMATO_IMPRESION_RANKING[] = " %i. %s\t\t\t%i\n",
            FORMATO_ENCABEZADO_RANKING[] = "JUGADOR\t\t\t\tPTOS.\n";

const float VELOCIDAD_DEFAULT = 1;

const int   LIMITE_ARGUMENTOS_RANKING = 4, LIMITE_ARGUMENTOS_CREAR_CAMINOS = 3,
            LIMITE_ARGUMENTOS_CREAR_CONFIGURACION = 3, LIMITE_ARGUMENTOS_VER_REPETICION = 4,
            LIMITE_ARGUMENTOS_JUGAR_PARTIDA = 4;

const char  LECTURA[] = "r", ESCRITURA[] = "w";

const char  RESISTENCIA_TORRES[] = "RESISTENCIA_TORRES=%i,%i\n",
            ENANOS_INICIO[] = "ENANOS_INICIO=%i,%i,%i,%i\n",
            ELFOS_INICIO[] = "ELFOS_INICIO=%i,%i,%i,%i\n",
            ENANOS_EXTRA[] = "ENANOS_EXTRA=%i,%i,%i\n",
            ELFOS_EXTRA[] = "ELFOS_EXTRA=%i,%i,%i\n",
            ENANOS_ANIMO[] = "ENANOS_ANIMO=%i,%i\n",
            ELFOS_ANIMO[] = "ELFOS_ANIMO=%i,%i\n",
            VELOCIDAD_CONFIG[] = "VELOCIDAD=%f\n",
            CAMINOS[] = "CAMINOS=%s\n";



void mostrar_comandos(char programa[]) {
    printf("Comandos:\n");
    printf("\n%s %s %s25 %smi_configuracion%s\n", programa, RANKING, LISTAR, CONFIGURACION, EXTENSION_CONFIGURACION);
    printf("\tSe muestra el ranking para la configuración dada y se muestra el top de la cantidad indicada.\n");
    printf("\tEn caso de no recibir configuración y/o lista, se toman los valores por defecto.\n");
    printf("\n%s %s mi_camino%s\n", programa, CREAR_CAMINOS, EXTENSION_CAMINOS);
    printf("\tSe permite crear caminos personalizados. Se guardarán en el archivo dado por eso es obligatorio.\n");
    printf("\tSi el archivo no existe, se creará, si existe se sobreescribirá.\n");
    printf("\n%s %s mi_configuracion%s\n", programa, CREAR_CONFIGURACION, EXTENSION_CONFIGURACION);
    printf("\tSe permite crear configuraciones personalizadas. Se guardarán en el archivo dado por eso es obligatorio.\n");
    printf("\tSi el archivo no existe, se creará, si existe se sobreescribirá.\n");
    printf("\n%s %s %smi_grabacion%s %s0.5\n", programa, VER_REPETICION, GRABACION, EXTENSION_GRABACION, VELOCIDAD);
    printf("\tPara ver la repetición de un juego es necesario indicar la grabación. La velocidad de reproducción es opcional\n");
    printf("\n%s %s %smi_configuracion%s %smi_grabacion%s\n", programa, JUGAR_PARTIDA, CONFIGURACION, EXTENSION_CONFIGURACION, GRABACION, EXTENSION_GRABACION);
    printf("\tSe pueden jugar configuraciones personalizadas. Para grabar la partida tiene que crear o sobreescribir una existente\n");
    printf("\tAmbos parámetros son opcionales\n");
}



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



bool ingresa_etiqueta(char archivo[], const char etiqueta[]) {
  return !strncmp(archivo, etiqueta, strlen(etiqueta));
}



//~ Pre: Recibe el nombre de un archivo y la extensión que se espera encontrar.
//~ Pos: Devuelve true si tiene dicha extensión.
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



bool ingresa_archivo(char archivo[]) {
  return strcmp(archivo, SIN_ARCHIVO) != 0;
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a RANKING
bool ingresa_ranking(char comando[]) {
    return !strcmp(comando, RANKING);
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a CREAR_CAMINOS
bool ingresa_crear_caminos(char comando[]) {
    return !strcmp(comando, CREAR_CAMINOS);
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a CREAR_CONFIGURACION
bool ingresa_crear_configuracion(char comando[]) {
    return !strcmp(comando, CREAR_CONFIGURACION);
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a VER_REPETICION
bool ingresa_ver_repeticion(char comando[]) {
    return !strcmp(comando, VER_REPETICION);
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a JUGAR_PARTIDA
bool ingresa_jugar_partida(char comando[]) {
    return !strcmp(comando, JUGAR_PARTIDA);
}



//~ Pre: Recibe los argumentos de la línea de comando junto a su tope. *cant_a_listar no está inicializado.
//~ Pos: Devuelve true si se ha ingresado LISTAR e inicializa *cant_a_listar con el valor numérico siguiente a LISTAR.
//~      *cant_a_listar tomará valores numéricos hasta encontrar uno no numérico. No comprueba la calidad del dato ingresado.
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



//~ Pre: Recibe los argumentos de la línea de comandos. archivo se encuentra sin inicializar.
//~ Pos: Devuelve true si se ha ingresado CONFIGURACION e iniciliza archivo con la cadena siguiente a CONFIGURACION.
//~      No se comprueba si archivo es un archivo de configuración válido (si existe, si se tienen permisos de lectura,
//~      ni si tiene EXTENSION_CONFIGURACION).
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



//~ Pre: Recibe los argumentos de la línea de comandos. archivo se encuentra sin inicializar.
//~ Pos: Devuelve true si se ha ingresado GRABACION e iniciliza archivo con la cadena siguiente a GRABACION.
//~      No se comprueba si archivo es un archivo de grabacion válido (si existe, si se tienen permisos de lectura,
//~      ni si tiene EXTENSION_GRABACION).
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



//~ Pre: Recibe los argumentos de la línea de comando junto a su tope. *velocidad no está inicializado.
//~ Pos: Devuelve true si se ha ingresado VELOCIDAD e inicializa *velocidad con el valor numérico siguiente a LISTAR.
//~      *velocidad tomará valores numéricos hasta encontrar uno no numérico. No comprueba la calidad del dato ingresado.
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



//~ Pre: Recibe el nombre de un archivo y la extensión esperada.
//~ Pos: Quita la extensión reemplazando el inicio de la extensión del archivo (suele ser '.') por NULL('\0').
void quitar_extension(char archivo[MAX_ARCHIVO], const char extension[]) {
    char* ext_archivo = strrchr(archivo, (int) *extension);
    if (ext_archivo != NULL)
        *ext_archivo = CARACTER_FINAL;
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
            printf("No se ingresó bien la cantidad a listar.\n");
            *es_valido = false;
        }
    }
    if (ingresa_configuracion(argc, argv, config)) {
        if (!tiene_extension_valida(config, EXTENSION_CONFIGURACION)) {
            quitar_extension(config, EXTENSION);
            printf("El archivo no tiene una extensión válida. Tiene que ser %s%s\n", config, EXTENSION_CONFIGURACION);
            *es_valido = false;
        }
    } else strcpy(config, SIN_ARCHIVO);
}




//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de CREAR_CAMINOS válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de caminos a crear o sobreescribir.
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
            printf("La extensión del archivo no es válida. Tiene que ser %s%s.\n", archivo, EXTENSION_CAMINOS);
            *es_valido = false;
        }
    }
}



//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de CREAR_CONFIGURACIÓN válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de configuración a crear o sobreescribir.
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




//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de VER_REPETICION válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de grabacion y la velocidad a reproducir.
void comprobar_ver_repeticion(int argc, char *argv[], char grabacion[MAX_ARCHIVO], float *velocidad, bool *es_valido) {
    if (argc > LIMITE_ARGUMENTOS_VER_REPETICION) {
        printf("Mucho argumento\n");
        *es_valido = false;
    } else if (!ingresa_argumento(argc) && !ingresa_grabacion(argc, argv, grabacion)) {
        printf("Tiene que ingresarse un archivo de grabacion%s\n", EXTENSION_GRABACION);
        *es_valido = false;
    }  else if (!tiene_extension_valida(grabacion, EXTENSION_GRABACION)) {
        quitar_extension(grabacion, EXTENSION);
        printf("La grabación no tiene una extensión válida. Tiene que ser %s%s\n", grabacion, EXTENSION_GRABACION);
        *es_valido = false;
    } else {
        if (!ingresa_velocidad(argc, argv, velocidad)) {
            *velocidad = VELOCIDAD_DEFAULT;
        } else if (!*velocidad) {
            printf("Hubo problemas con la velocidad ingresada\n");
        }
    }
}



//~ Pre: Recibe todos los argumentos de la línea de comandos junto a su tope. Se presupone que son argumentos de JUGAR_PARTIDA válidos.
//~ Pos: Se comprueba la validez de los argumentos ingresados. Cuando se encuentra un error, se imprime un mensaje en pantalla
//~      y *es_valido cambia su valor a falso. Se obtiene nombre del archivo de configuración y de grabación.
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
        } else strcpy(config, SIN_ARCHIVO);;

        if (ingresa_grabacion(argc, argv, grabacion)) {
            if (!tiene_extension_valida(grabacion, EXTENSION_GRABACION)) {
                quitar_extension(grabacion, EXTENSION);
                printf("La grabación no está con la extensión adecuada. Tiene que ser %s%s\n", grabacion, EXTENSION_GRABACION);
                *es_valido = false;
            };
        } else strcpy(grabacion, SIN_ARCHIVO);;
    }
}



//~ Devuelve true si archivo pudo ser inicializado. Si no, muestra en pantalla que no se pudo abrir el archivo.
bool se_puede_abrir(FILE* archivo, char nombre[MAX_ARCHIVO]) {
    if(!archivo) {
        printf("No se pudo abrir %s, campeón\n", nombre);
    }
    return archivo;
}



//~ Pre: Recibe el nombre de un archivo de configuración.
//~ Pos: Inicializa ranking con el nombre del archivo de una configuración personalizada o predeterminada.
void obtener_ranking(char ranking[MAX_ARCHIVO], char config[MAX_ARCHIVO]) {
    strcpy(ranking, ARCHIVO_RANKING);
    if (ingresa_archivo(config)) {
        quitar_extension(config, EXTENSION_CONFIGURACION);
        strcat(ranking, SEPARACION);
        strcat(ranking, config);
    }
    strcat(ranking, EXTENSION_RANKING);
}



void grabar_rank_aux(char ranking[], rank_t rank) {
    int leido;
    rank_t rank_aux;
    FILE *archivo, *archivo_aux;
    archivo = fopen(ranking, LECTURA);
    if (!se_puede_abrir(archivo, ranking)) return;
    archivo_aux = fopen(ARCHIVO_RANKING_AUX, ESCRITURA);
    if (!se_puede_abrir(archivo_aux, (char*) ARCHIVO_RANKING_AUX)) {
        fclose(archivo);
        return;
    }
    leido = fscanf(archivo, FORMATO_LECTURA_RANKING, rank_aux.nombre, &(rank_aux.puntaje));
    while (!leido) {
        if (rank.puntaje > rank_aux.puntaje) {
            fprintf(archivo_aux, FORMATO_ESCRITURA_RANKING, rank.nombre, rank.puntaje);
            rank = rank_aux;
        }
        leido = fscanf(archivo, FORMATO_LECTURA_RANKING, rank_aux.nombre, &(rank_aux.puntaje));
    }
    fclose(archivo_aux);
    fclose(archivo);
    rename(ARCHIVO_RANKING_AUX, ranking);
}



void grabar_rank(char config[], rank_t rank) {
    grabar_rank_aux(config, rank);
    if (ingresa_archivo(config)) grabar_rank((char *) SIN_ARCHIVO, rank);
}



void crear_camino(int nivel_actual) {
    /*
     * crear_camino(1)
     * crear_camino(2)
     * crear_camino(3)
     * crear_camino(4)
     */
}
void inicializar_configuracion(configuracion_t *configuracion) {
    configuracion->max_niveles = MAX_NIVELES;
    configuracion->juego.torres.resistencia_torre_1 = INDEFINIDO;
    configuracion->juego.torres.resistencia_torre_2 = INDEFINIDO;
    configuracion->juego.torres.enanos_extra = INDEFINIDO;
    configuracion->juego.torres.elfos_extra = INDEFINIDO;
    configuracion->juego.torres.elfos_extra = INDEFINIDO;
    configuracion->juego.critico_gimli = INDEFINIDO;
    configuracion->juego.critico_legolas = INDEFINIDO;
    configuracion->juego.fallo_gimli = INDEFINIDO;
    configuracion->juego.fallo_legolas = INDEFINIDO;
    for (int i = 0; i < configuracion->max_niveles; i++) {
        configuracion->enanos_inicio[i] = INDEFINIDO;
        configuracion->elfos_inicio[i] = INDEFINIDO;
    }
    configuracion->coste_enanos_torre_1 = INDEFINIDO;
    configuracion->coste_enanos_torre_2 = INDEFINIDO;
    configuracion->coste_elfos_torre_1 = INDEFINIDO;
    configuracion->coste_elfos_torre_2 = INDEFINIDO;
    configuracion->velocidad = INDEFINIDO;
    strcpy(configuracion->caminos, "-1");
}



void crear_configuracion() {
    /*
     * preguntar_resistencia_torres();
     * preguntar enanos por nivel
     * preguntar elfos por nivel
     * preguntar enanos extra, coste a las torres
     * preguntar elfos extra coste a las torres
     * preguntar fallo y critico enanos
     * preguntar fallo y critico elfos
     * preguntar velocidad
     * preguntar caminos
     */
}



void escribir_configuracion(configuracion_t configuracion, FILE* archivo) {
    juego_t juego = configuracion.juego;
    torres_t torres = juego.torres;
    fprintf(archivo, RESISTENCIA_TORRES, torres.resistencia_torre_1, torres.resistencia_torre_2);
    fprintf(archivo, ENANOS_INICIO, configuracion.enanos_inicio[0], configuracion.enanos_inicio[1], configuracion.enanos_inicio[2], configuracion.enanos_inicio[3]);
    fprintf(archivo, ELFOS_INICIO, configuracion.elfos_inicio[0], configuracion.elfos_inicio[1], configuracion.elfos_inicio[2], configuracion.elfos_inicio[3]);
    fprintf(archivo, ENANOS_EXTRA, torres.enanos_extra, configuracion.coste_enanos_torre_1, configuracion.coste_enanos_torre_2);
    fprintf(archivo, ELFOS_EXTRA, torres.elfos_extra, configuracion.coste_elfos_torre_1, configuracion.coste_elfos_torre_2);
    fprintf(archivo, ENANOS_ANIMO, juego.fallo_gimli, juego.critico_gimli);
    fprintf(archivo, ELFOS_ANIMO, juego.fallo_legolas, juego.critico_legolas);
    fprintf(archivo, VELOCIDAD_CONFIG, configuracion.velocidad);
    fprintf(archivo, CAMINOS, configuracion.caminos);
}



void cargar_configuracion(configuracion_t *configuracion, FILE* archivo) {
    //~ juego_t juego = (*configuracion).juego;
    //~ torres_t torres = juego.torres;
    //~ if (!fscanf(archivo, RESISTENCIA_TORRES, &(torres.resistencia_torre_1), &(torres.resistencia_torre_2))) {
        //~ torres.resistencia_torre_1 = INDEFINIDO;
        //~ torres.resistencia_torre_2 = INDEFINIDO;
    //~ }
    //~ if (!fscanf(archivo, ENANOS_INICIO, &((*configuracion).enanos_inicio[0]), &((*configuracion).enanos_inicio[1]), &((*configuracion).enanos_inicio[2]), &((*configuracion).enanos_inicio[3]))) {
        //~ configuracion->enanos_inicio[0] = INDEFINIDO;
        //~ configuracion->enanos_inicio[1] = INDEFINIDO;
        //~ configuracion->enanos_inicio[2] = INDEFINIDO;
        //~ configuracion->enanos_inicio[3] = INDEFINIDO;
    //~ };
    //~ if (!fscanf(archivo, ELFOS_INICIO, &((*configuracion).elfos_inicio[0]), (&(*configuracion).elfos_inicio[1]), &((*configuracion).elfos_inicio[2]), &((*configuracion).elfos_inicio[3]))) {
        //~ configuracion->elfos_inicio[0] = INDEFINIDO;
        //~ configuracion->elfos_inicio[1] = INDEFINIDO;
        //~ configuracion->elfos_inicio[2] = INDEFINIDO;
        //~ configuracion->elfos_inicio[3] = INDEFINIDO;
    //~ }
    //~ if (!fscanf(archivo, ENANOS_EXTRA, &(torres.enanos_extra), &((*configuracion).coste_enanos_torre_1), &((*configuracion).coste_enanos_torre_2))) {
        //~ torres.enanos_extra = INDEFINIDO;
        //~ configuracion->coste_enanos_torre_1 = INDEFINIDO;
        //~ configuracion->coste_enanos_torre_2 = INDEFINIDO;
    //~ }
    //~ fscanf(archivo, ELFOS_EXTRA, &(torres.elfos_extra), &((*configuracion).coste_elfos_torre_1), &((*configuracion).coste_elfos_torre_2));
    //~ fscanf(archivo, ENANOS_ANIMO, &(juego.fallo_gimli), &(juego.critico_gimli));
    //~ fscanf(archivo, ELFOS_ANIMO, &(juego.fallo_legolas), &(juego.critico_legolas));
    //~ fscanf(archivo, VELOCIDAD_CONFIG, &((*configuracion).velocidad));
    //~ fscanf(archivo, CAMINOS, (*configuracion).caminos);
}



void ejecutar_ranking(int cant_a_listar, char config[MAX_ARCHIVO]) {
    int i = 1, leido;
    char ranking[MAX_ARCHIVO];
    rank_t rank;
    obtener_ranking(ranking, config);
    FILE* archivo = fopen(ranking, LECTURA);
    if (!se_puede_abrir(archivo, ranking)) {
        printf("Tal vez tengas que jugar unas veces para generar un ranking.\n");
        return;
    }
    leido = fscanf(archivo, FORMATO_LECTURA_RANKING, rank.nombre, &rank.puntaje);
    printf(FORMATO_ENCABEZADO_RANKING);
    while ((leido != EOF) && (i < cant_a_listar || cant_a_listar == INDEFINIDO)) {
        printf(FORMATO_IMPRESION_RANKING, i, rank.nombre, rank.puntaje);
        leido = fscanf(archivo, FORMATO_LECTURA_RANKING, rank.nombre, &rank.puntaje);
        i++;
    }
    fclose(archivo);
}




void ejecutar_crear_caminos(char caminos[MAX_ARCHIVO]) {
    FILE* archivo = fopen(caminos, ESCRITURA);
    if (!se_puede_abrir(archivo, caminos)) return;
    /*
     * crear caminos
     * escribir_caminos
     */
    fclose(archivo);
}




void ejecutar_crear_configuracion(char config[MAX_ARCHIVO]) {
    FILE* archivo = fopen(config, ESCRITURA);
    if (!se_puede_abrir(archivo, config)) return;
    configuracion_t configuracion;
    inicializar_configuracion(&configuracion);
    crear_configuracion(&configuracion);
    escribir_configuracion(configuracion, archivo);
    fclose(archivo);
}




void ejecutar_ver_repeticion(char grabacion[MAX_ARCHIVO], float velocidad) {
    FILE* archivo = fopen(grabacion, LECTURA);
    if (!se_puede_abrir(archivo, grabacion)) return;
    reproducir(archivo, velocidad);
    fclose(archivo);
}




void ejecutar_jugar_partida(char config[MAX_ARCHIVO], char grabacion[MAX_ARCHIVO]) {
    FILE *archivo_config, *archivo_grabacion;
    configuracion_t configuracion;
    rank_t rank;

    inicializar_configuracion(&configuracion);
    if (ingresa_archivo(config)) {
        archivo_config = fopen(config, LECTURA);
        if (!se_puede_abrir(archivo_config, config)) {
            printf("Probá con otra configuración.\n");
            return;
        }
        cargar_configuracion(&configuracion, archivo_config);
        fclose(archivo_config);
    }

    if (ingresa_archivo(grabacion)) {
        archivo_grabacion = fopen(grabacion, ESCRITURA);
        if (!se_puede_abrir(archivo_grabacion, grabacion)) {
          printf("Ocurrió un problema y no se puede grabar el juego.\n");
          return;
        }
    }
    iniciar_juego(configuracion, archivo_grabacion, &rank);
    if (archivo_grabacion != NULL) fclose(archivo_grabacion);

    grabar_rank(config, rank);
    ejecutar_ranking(INDEFINIDO, config);
}



int main(int argc, char *argv[]) {
    if (!ingresa_comando(argc)) {
        mostrar_comandos(argv[PROGRAMA]);
        return 0;
    }

    bool es_valido = true;
    char config[MAX_ARCHIVO], caminos[MAX_ARCHIVO], grabacion[MAX_ARCHIVO];
    int cant_a_listar; float velocidad;

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

    return 0;
}
