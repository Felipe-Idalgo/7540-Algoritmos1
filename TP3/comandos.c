#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARCHIVO 100

const int   PROGRAMA = 0, COMANDO = 1, ARGUMENTO = 2;

const char  RANKING[] = "ranking", CREAR_CAMINOS[] = "crear_camino",
            CREAR_CONFIGURACION[] = "crear_configuracion", VER_REPETICION[] = "poneme_la_repe",
            JUGAR_PARTIDA[] = "jugar";

const char  LISTAR[] = "listar=", CONFIGURACION[] = "config=",
            GRABACION[] = "grabacion=", VELOCIDAD[] = "velocidad=";

const char  SIN_ARCHIVO[] = "", CARACTER_FINAL = '\0';

const char  EXTENSION_RANKING[] = ".csv", EXTENSION_CAMINOS[] = ".txt",
            EXTENSION_CONFIGURACION[] = ".txt", EXTENSION_GRABACION[] = ".dat";

const char  ARCHIVO_CAMINOS[] = "%s.txt",
            ARCHIVO_CONFIGURACION[] = "%s.txt",
            ARCHIVO_CONFIGURACION_DEFAULT[] = "default.txt",
            ARCHIVO_RANKING[] = "ranking";

const int   LIMITE_ARGUMENTOS_RANKING = 4, LIMITE_ARGUMENTOS_CREAR_CAMINOS = 3,
            LIMITE_ARGUMENTOS_CREAR_CONFIGURACION = 3, LIMITE_ARGUMENTOS_VER_REPETICION = 4,
            LIMITE_ARGUMENTOS_JUGAR_PARTIDA = 4;

const char  LECTURA[] = "r", ESCRITURA[] = "w";



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
//~      No se comprueba si archivo es un archivo de configuración válido (si existe, si se tienen permisos de lectura,
//~      ni si tiene EXTENSION_CONFIGURACION).
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
        if (!tiene_extension_valida(config, EXTENSION_CONFIGURACION)) {
            printf("No se ingresó bien la configuración.\n");
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
        if (!tiene_extension_valida(archivo, EXTENSION_CAMINOS)) {
            printf("No se ingresó una extensión válida\n");
            *es_valido = false;
        }
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
        if (!tiene_extension_valida(archivo, EXTENSION_CONFIGURACION)) {
            printf("No se ingresó una extensión válida\n");
            *es_valido = false;
        }
    }
}



//~ Pre: Recibe el argumento que esté en la posición de COMANDO.
//~ Pos: Devuelve true si el argumento corresponde a VER_REPETICION
bool ingresa_ver_repeticion(char comando[]) {
    return !strcmp(comando, VER_REPETICION);
}



//~ Pre: Recibe los argumentos de la línea de comandos. archivo se encuentra sin inicializar.
//~ Pos: Devuelve true si se ha ingresado GRABACION e iniciliza archivo con la cadena siguiente a GRABACION.
//~      No se comprueba si archivo es un archivo de grabacion válido (si existe, si se tienen permisos de lectura,
//~      ni si tiene EXTENSION_GRABACION).
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
        || !tiene_extension_valida(grabacion, EXTENSION_GRABACION)
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
        if (ingresa_configuracion(argc, argv, config) && tiene_extension_valida(config, EXTENSION_CONFIGURACION)) {
            printf("Configuracion: %s\n", config);
        } else {
            printf("Configuracion default\n");
        }
        if (ingresa_grabacion(argc, argv, grabacion) && tiene_extension_valida(grabacion, EXTENSION_GRABACION)) {
            printf("Grabacion: %s\n", grabacion);
        }
    }
}



//~ Pre: Recibe el nombre de un archivo y la extensión esperada.
//~ Pos: Quita la extensión reemplazando el inicio de la extensión del archivo (suele ser '.') por NULL('\0').
void quitar_extension(char archivo[MAX_ARCHIVO], const char extension[]) {
    char* ext_archivo = strrchr(archivo, (int) *extension);
    *ext_archivo = CARACTER_FINAL;
}



//~ Devuelve true si archivo pudo ser inicializado. Si no, muestra en pantalla que no se pudo abrir el archivo.
bool se_puede_abrir(FILE* archivo, char nombre[MAX_ARCHIVO]) {
    if(!archivo) {
        printf("No se pudo abrir %s, campeón\n", nombre);
    }
    return archivo;
}



//~ Le da el formato a ranking desde la configuracion para poder obtener el archivo donde se almacenó el rank
void obtener_ranking(char rank[MAX_ARCHIVO], char config[MAX_ARCHIVO]) {
    strcpy(rank, ARCHIVO_RANKING);
    if (strcmp(config, ARCHIVO_CONFIGURACION_DEFAULT)) {
        quitar_extension(config, EXTENSION_CONFIGURACION);
        strcat(rank, "_");
        strcat(rank, config);
    }
    strcat(rank, EXTENSION_RANKING);
}



void ejecutar_ranking(int cant_a_listar, char config[MAX_ARCHIVO]) {
    char archivo_ranking[MAX_ARCHIVO];
    obtener_ranking(archivo_ranking, config);
    FILE* archivo = fopen(archivo_ranking, LECTURA);
    if (!se_puede_abrir(archivo, archivo_ranking)) return;
    /*
     * mostrar ranking
     */
    fclose(archivo);
}

void ejecutar_crear_caminos(char caminos[MAX_ARCHIVO]) {
    FILE* archivo = fopen(caminos, ESCRITURA);
    if (!se_puede_abrir(archivo, caminos)) return;
    /*
     * crear caminos
     */
    fclose(archivo);
}

void ejecutar_crear_configuracion(char config[MAX_ARCHIVO]) {
    FILE* archivo = fopen(config, ESCRITURA);
    if (!se_puede_abrir(archivo, config)) return;
    /*
     * crear configuracion
     */
    fclose(archivo);
}

void ejecutar_ver_repeticion(char grabacion[MAX_ARCHIVO], float velocidad) {
    FILE* archivo = fopen(grabacion, LECTURA);
    if (!se_puede_abrir(archivo, grabacion)) return;
    /*
     * ver repeticion
     */
    fclose(archivo);
}

void ejecutar_jugar_partida(char config[MAX_ARCHIVO], char grabacion[MAX_ARCHIVO]) {
    FILE* archivo_config = fopen(config, LECTURA);
    if (!se_puede_abrir(archivo_config, config)) return;
    /*
     * cargar configuracion
     */
    fclose(archivo_config);

    FILE* archivo_grabacion = fopen(grabacion, ESCRITURA);
    if (!se_puede_abrir(archivo_grabacion, grabacion)) return;
    /*
     * jugar y grabar
     */
    fclose(archivo_grabacion);
}



int main(int argc, char *argv[]) {
    if (!ingresa_comando(argc)) {
        mostrar_comandos(argv[PROGRAMA]);
        return 0;
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
        comprobar_ranking(argc, argv, config, &cant_a_listar, &es_valido);
        if (es_valido) ejecutar_ranking(cant_a_listar, config);
    } else if (ingresa_crear_caminos(argv[COMANDO])) {
        /*
         * ./defendiendo crear_camino mis_caminos.txt
         * archivo obligatorio
         * mostrar en pantalla como va quedado
         */
        comprobar_crear_caminos(argc, argv, caminos, &es_valido);
        if (es_valido) ejecutar_crear_caminos(caminos);
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
        comprobar_crear_configuracion(argc, argv, config, &es_valido);
        if (es_valido) ejecutar_crear_configuracion(config);
    } else if (ingresa_ver_repeticion(argv[COMANDO])) {
        /*
         * ./defendiendo poneme_la_repe grabacion=mi_partida.dat velocidad=0.5
         * grabacion obligatoria, velocidad opcional
         * velocidad solicitada o 1 (default)
         */
        comprobar_ver_repeticion(argc, argv, grabacion, &velocidad, &es_valido);
        if (es_valido) ejecutar_ver_repeticion(grabacion, velocidad);
    } else if (ingresa_jugar_partida(argv[COMANDO])) {
        /*
         * ./defendiendo jugar config=mi_configuracion.txt grabacion=mi_partida.dat
         * si el archivo no existe o no fue ingresado: config_default
         * grabacion: (sobre)escribe si fue ingresado, si no no se graba
         */
        comprobar_jugar_partida(argc, argv, config, grabacion, &es_valido);
        if (es_valido) ejecutar_jugar_partida(config, grabacion);
    } else {
        printf("No se ingreso ningun comando correcto.\n");
    }

    return 0;
}
 
