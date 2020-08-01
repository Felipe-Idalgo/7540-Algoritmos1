#include "defendiendo_torres.h"
#include "archivos.h"
#include "configuracion.h"
#include <string.h>

#define ERROR -1

#define ENANOS 'G'
#define ELFOS 'L'
#define TORRE_1 1
#define TORRE_2 2
#define CAMINOS_INDEFINIDOS "-1"

const int   MAX_DATO = 10,
            MAX_RESISTENCIA = 1000,
            MAX_DEFENSORES_EXTRAS = 20,
            MAX_DEFENSORES_INICIO = 20,
            MAX_CRITICO = 100,
            MAX_FALLO = 100;
const float MAX_VELOCIDAD = 2.0;

const char  RESISTENCIA_TORRES[] = "RESISTENCIA_TORRES=%i,%i\n",
            ENANOS_INICIO[] = "ENANOS_INICIO=%i,%i,%i,%i\n",
            ELFOS_INICIO[] = "ELFOS_INICIO=%i,%i,%i,%i\n",
            ENANOS_EXTRA[] = "ENANOS_EXTRA=%i,%i,%i\n",
            ELFOS_EXTRA[] = "ELFOS_EXTRA=%i,%i,%i\n",
            ENANOS_ANIMO[] = "ENANOS_ANIMO=%i,%i\n",
            ELFOS_ANIMO[] = "ELFOS_ANIMO=%i,%i\n",
            VELOCIDAD_CONFIG[] = "VELOCIDAD=%f\n",
            CAMINOS[] = "CAMINOS=%s\n";

/*
 * Pre: Recibe el valor numérico de un dato ingresado por el usuario.
 * Pos: Devuelve true si se ha ingresado un número mayor a 0 y menor a MAX_RESISTENCIA.
 *      Se permite la opción INDEFINIDO para vlores predeterminados del juego.
 */
bool es_resistencia_valida(int dato_num) {
    return (dato_num) && (dato_num >= INDEFINIDO) && (dato_num < MAX_RESISTENCIA);
}

/*
 * Pre: Recibe el valor numérico de un dato ingresado por el usuario.
 * Pos: Devuelve true si el valor numérico se encuentra entre 0 y MAX_DEFENSORES_EXTRAS.
 *      Se permite la opción INDEFINIDO para valores predeterminados del juego.
 */
bool es_defensa_extra_valida(int dato_num) {
    return (dato_num >= INDEFINIDO) && (dato_num < MAX_DEFENSORES_EXTRAS);
}

/*
 * Pre: Recibe el valor numérico de un dato ingresado por el usuario.
 * Pos: Devuelve true si el valor numérico se encuentra entre 0 y MAX_DEFENSORES_INICIO.
 *      Se permite la opción INDEFINIDO para valores predeterminados del juego.
 */
bool es_defensa_inicial_valida(int dato_num) {
    return (dato_num >= INDEFINIDO) && (dato_num < MAX_DEFENSORES_INICIO);
}

/*
 * Pre: Recibe el valor númerico de un dato ingresado por el usuario.
 * Pos: Devuelve true si el valor se encuentra entre 0 y MAX_RESISTENCIA_INICIO.
 *      Se permite INDEFINIDO para usar los valores predeterminados del juego.
 */
bool es_costo_valido(int dato_num) {
    return es_resistencia_valida(dato_num) || !dato_num;
}

/*
 * Pre: Recibe el valor numérico de un dato ingresado por el usuario.
 * Pos: Devuelve true si el valor se encuentra entre 0 y MAX_CRITICO.
 *      Se permite la opción INDEFINIDO para valores predeterminados del juego.
 */
bool es_critico_valido(int dato_num) {
    return (dato_num >= INDEFINIDO) && (dato_num < MAX_CRITICO);
}

/*
 * Pre: Recibe el valor numérico de un dato ingresado por el usuario.
 * Pos: Devuelve true si el valor se encuentra entre 0 y MAX_FALLO.
 *      Se permite la opción INDEFINIDO para valores predeferminados del juego
 */
bool es_fallo_valido(int dato_num) {
    return (dato_num >= INDEFINIDO) && (dato_num < MAX_FALLO);
}

/*
 * Pre: Recibe el valor decimal del dato ingresado por el usuario.
 * Pos: Devuelve true si el valor es mayor a 0 y menor a MAX_VELOCIDAD.
 *      Se permite la opción INDEFINIDO para el valor predeterminado del juego.
 */
bool es_velocidad_valida(double dato_num) {
    return (dato_num) && (dato_num >= INDEFINIDO) && (dato_num < MAX_VELOCIDAD);
}

/*
 * Pre: Recibe una cadena ingresada por el usuario.
 * Pos: Devuelve true si el usuario ha ingresado algo en la cadena.
 */
bool hay_archivo(char archivo[]) {
    return strcmp(archivo, SIN_ARCHIVO);
}

/*
 * Pre: Recibe por referencia la variable donde se almacenara el valor
 *      de la resistencia de la torre, y el número de la torre que se pide.
 * Pos: Muestra en pantalla un mensaje donde le pide al usuario ingresar los valores.
 *      Se obtienen valores válidos.
 */
void pedir_resistencia_torre(int *resistencia_torre, int torre) {
    int dato_num;
    char dato[MAX_DATO];
    printf("Ingrese resistencia para la Torre %i: ", torre);
    scanf("%s", dato);
    dato_num = atoi(dato);
    while (!es_resistencia_valida(dato_num)) {
        printf("Hubo un problema. Ingresa un valor entre %i y %i: ", INDEFINIDO, MAX_RESISTENCIA);
        scanf("%s", dato);
        dato_num = atoi(dato);
    }
    *resistencia_torre = dato_num;
}

/*
 * Pide los valores de resistencia que quiera ingresar el usuario y los guarda en
 * una configuracion.
 */
void obtener_resistencia_torres(configuracion_t *configuracion) {
    pedir_resistencia_torre(&(configuracion->juego.torres.resistencia_torre_1), TORRE_1);
    pedir_resistencia_torre(&(configuracion->juego.torres.resistencia_torre_2), TORRE_2);
}

/*
 * Pre: Recibe por referencia la variable donde se almacenará la cantidad de
 *      defensores extras, y el tipo que se quiera configurar.
 * Pos: Se pide al usuario ingresar la cantidad de defensores que desee configurar.
 *      Se obtendrán cantidades válidas.
 */
void pedir_defensores_extras(int *defensores_extras, char tipo) {
    int dato_num;
    char dato[MAX_DATO];
    printf("Ingrese nº de defensores (%c) extras para las torres: ", tipo);
    scanf("%s", dato);
    dato_num = atoi(dato);
    while (!es_defensa_extra_valida(dato_num)) {
        printf("Hubo un problema. Ingresa un valor entre %i y %i: ", INDEFINIDO, MAX_DEFENSORES_EXTRAS);
        scanf("%s", dato);
        dato_num = atoi(dato);
    }
    *defensores_extras = dato_num;
}

/*
 * Pide al usuario que ingrese las cantidades de defensores extras con las que
 * desee jugar y se guardarán en la configuración.
 */
void obtener_defensores_extras(configuracion_t *configuracion) {
    pedir_defensores_extras(&(configuracion->juego.torres.enanos_extra), ENANOS);
    pedir_defensores_extras(&(configuracion->juego.torres.elfos_extra), ELFOS);
}

/*
 * Pre: Recibe por referencia la variable donde se almacenarán los defensores
 *      iniciales para determinado nivel y de determinado tipo.
 * Pos: Se pide al usuario ingresar las cantidades de defensores que desee
 *      configurar. Se obtendrán valores válidos.
 */
void pedir_defensores_inicio(int *defensores_inicio, char tipo, int nivel) {
    int dato_num;
    char dato[MAX_DATO];
    printf("Ingrese nº de defensores (%c) para nivel %i: ", tipo, nivel);
    scanf("%s", dato);
    dato_num = atoi(dato);
    while (!es_defensa_inicial_valida(dato_num)) {
        printf("Hubo un problema. Ingresa un valor entre %i y %i: ", INDEFINIDO, MAX_DEFENSORES_INICIO);
        scanf("%s", dato);
        dato_num = atoi(dato);
    }
    *defensores_inicio = dato_num;
}

/*
 * Permite al usuario configurar la cantidad de defensores iniciales que quiera
 * por cada nivel y de cada tipo. Los datos se almacenarán en la configuración.
 */
void obtener_defensores_inicio(configuracion_t *configuracion) {
    for (int nivel = 1; nivel <= MAX_NIVELES; nivel++) {
        pedir_defensores_inicio(&(configuracion->enanos_inicio[nivel]), ENANOS, nivel);
        pedir_defensores_inicio(&(configuracion->elfos_inicio[nivel]), ENANOS, nivel);
    }
}

/*
 * Pre: Recibe por referencia el costo que tendra un defensor de determinado tipo
 *      para una de las torres.
 * Pos: Se le pedirá al usuario que ingrese el costo que tendrán los defensores
 *      de dicho tipo para dicha torre. El valor obtenido será válido.
 */
void pedir_costo_defensores(int *costo, char tipo, int torre) {
    int dato_num;
    char dato[MAX_DATO];
    printf("Ingrese costo de %c para Torre %i: ", tipo, torre);
    scanf("%s", dato);
    dato_num = atoi(dato);
    while (!es_costo_valido(dato_num)) {
        printf("Hubo un problema. Ingresa un valor entre %i y %i: ", INDEFINIDO, MAX_RESISTENCIA);
        scanf("%s", dato);
        dato_num = atoi(dato);
    }
    *costo = dato_num;
}

/*
 * Permite al usuario configurar el costo de los defensores y almacenar
 * dichos datos en la configuración.
 */
void obtener_costo_defensores(configuracion_t *configuracion) {
    pedir_costo_defensores(&(configuracion->costo_enanos_torre_1), ENANOS, TORRE_1);
    pedir_costo_defensores(&(configuracion->costo_enanos_torre_2), ENANOS, TORRE_2);
    pedir_costo_defensores(&(configuracion->costo_elfos_torre_1), ELFOS, TORRE_1);
    pedir_costo_defensores(&(configuracion->costo_elfos_torre_2), ELFOS, TORRE_2);
}

/*
 * Pre: Recibe por referencia la probabilidad de dar un ataque crítico
 *      un defensor de dicho tipo.
 * Pos: El usuario podrá ingresar la probabilidad que desee. Dicho dato
 *      será válido.
 */
void pedir_critico(int *critico, char tipo) {
    int dato_num;
    char dato[MAX_DATO];
    printf("Ingrese porcentaje de criticidad de %c: ", tipo);
    scanf("%s", dato);
    dato_num = atoi(dato);
    while (!es_critico_valido(dato_num)) {
        printf("Hubo un problema. Ingresa un valor entre %i y %i: ", INDEFINIDO, MAX_CRITICO);
        scanf("%s", dato);
        dato_num = atoi(dato);
    }
    *critico = dato_num;
}

/*
 * Se pide al usuario ingresar la probabilidad de dar un ataque crítico
 * por cada defensor y los datos se guardarán en la configuración.
 */
void obtener_critico(configuracion_t *configuracion) {
    pedir_critico(&(configuracion->juego.critico_gimli), ENANOS);
    pedir_critico(&(configuracion->juego.critico_legolas), ELFOS);
}

/*
 * Pre: Recibe por referencia la probabilidad de fallar un ataque para
 *      un defensor de dicho tipo.
 * Pos: El usuario podrá ingresar la probabilidad que desee. Dicho dato
 *      será válido.
 */
void pedir_fallo(int *fallo, char tipo) {
    int dato_num;
    char dato[MAX_DATO];
    printf("Ingrese porcentaje de fallo de %c: ", tipo);
    scanf("%s", dato);
    dato_num = atoi(dato);
    while (!es_fallo_valido(dato_num)) {
        printf("Hubo un problema. Ingresa un valor entre %i y %i: ", INDEFINIDO, MAX_FALLO);
        scanf("%s", dato);
        dato_num = atoi(dato);
    }
    *fallo = dato_num;
}

/*
 * Se pide al usuario ingresar la probabilidad de fallar un ataque para
 * cada defensor y los datos se guardarán en la configuración.
 */
void obtener_fallo(configuracion_t *configuracion) {
    pedir_fallo(&(configuracion->juego.fallo_gimli), ENANOS);
    pedir_fallo(&(configuracion->juego.fallo_legolas), ELFOS);
}

/*
 * Se pide al usuario ingresar el intervalo de tiempo entre turnos.
 * El valor se almacenará en la configuración.
 */
void pedir_velocidad(configuracion_t *configuracion) {
    double dato_num;
    char dato[MAX_DATO];
    printf("Ingrese tiempo entre turnos: ");
    scanf("%s", dato);
    dato_num = atof(dato);
    while (!es_velocidad_valida(dato_num)) {
        printf("Hubo un problema. Ingresa un valor entre %d y %f: ", INDEFINIDO, MAX_VELOCIDAD);
        scanf("%s", dato);
        dato_num = atof(dato);
    }
    configuracion->velocidad = (float) dato_num;
}

/*
 * Se pide al usuario ingresar el archivo donde se encuentran los caminos a utilizar.
 * Se guarda el nombre del archivo o INDEFINIDO si se desea usar el modo predeterminado
 * del juego, pero no se comprueba la existencia de dicho archivo ni si se tienen los permisos
 * para leerlo.
 */
void pedir_camino(configuracion_t *configuracion) {
    char dato[MAX_DATO];
    printf("Ingrese un camino: ");
    scanf("%s", dato);
    while (!hay_archivo(dato)) {
        printf("Hubo un problema. Probá nuevamente: ");
        scanf("%s", dato);
    }
    strcpy(configuracion->caminos, dato);
}

/*
 * Muestra en pantalla todos los mensajes para que el usuario pueda crear
 * una configuración nueva con valores válidos.
 */
void crear_configuracion(configuracion_t *configuracion) {
    printf("\nIngrese los valores deseables, y para dejar los valores por defecto ingrese %i\n", INDEFINIDO);
    obtener_resistencia_torres(configuracion);
    obtener_defensores_extras(configuracion);
    obtener_defensores_inicio(configuracion);
    obtener_defensores_inicio(configuracion);
    obtener_costo_defensores(configuracion);
    obtener_critico(configuracion);
    obtener_fallo(configuracion);
    pedir_velocidad(configuracion);
    pedir_camino(configuracion);
}

/*
 * Inicializa la configuración cuando no se ha leído un archivo de configuración.
 */
void inicializar_configuracion(configuracion_t *configuracion) {
    configuracion->juego.torres.resistencia_torre_1 = INDEFINIDO;
    configuracion->juego.torres.resistencia_torre_2 = INDEFINIDO;
    configuracion->juego.torres.enanos_extra = INDEFINIDO;
    configuracion->juego.torres.elfos_extra = INDEFINIDO;
    configuracion->juego.critico_gimli = INDEFINIDO;
    configuracion->juego.critico_legolas = INDEFINIDO;
    configuracion->juego.fallo_gimli = INDEFINIDO;
    configuracion->juego.fallo_legolas = INDEFINIDO;
    for (int i = 0; i < MAX_NIVELES; i++) {
        configuracion->enanos_inicio[i] = INDEFINIDO;
        configuracion->elfos_inicio[i] = INDEFINIDO;
    }
    configuracion->costo_enanos_torre_1 = INDEFINIDO;
    configuracion->costo_enanos_torre_2 = INDEFINIDO;
    configuracion->costo_elfos_torre_1 = INDEFINIDO;
    configuracion->costo_elfos_torre_2 = INDEFINIDO;
    configuracion->velocidad = INDEFINIDO;
    strcpy(configuracion->caminos, CAMINOS_INDEFINIDOS);
}

/*
 * Pre: Recibe la configuración que se desee guardar y el nombre del archivo que se utilizará
 * Pos: Si el archivo no existe se creará. Si existe se sobreescribirá. En él se guardarán
 *     los datos de la configuración recibida.
 */
int escribir_configuracion(configuracion_t configuracion, char config[MAX_ARCHIVO]) {
    FILE * archivo;
    juego_t juego;
    torres_t torres;
    archivo = fopen(config, ESCRITURA);
    if (!archivo) {
        printf("No estoy teniendo permisos suficientes para escribir.\n");
        return ERROR;
    }
    juego = configuracion.juego;
    torres = juego.torres;
    fprintf(archivo, RESISTENCIA_TORRES, torres.resistencia_torre_1, torres.resistencia_torre_2);
    fprintf(archivo, ENANOS_INICIO, configuracion.enanos_inicio[0], configuracion.enanos_inicio[1], configuracion.enanos_inicio[2], configuracion.enanos_inicio[3]);
    fprintf(archivo, ELFOS_INICIO, configuracion.elfos_inicio[0], configuracion.elfos_inicio[1], configuracion.elfos_inicio[2], configuracion.elfos_inicio[3]);
    fprintf(archivo, ENANOS_EXTRA, torres.enanos_extra, configuracion.costo_enanos_torre_1, configuracion.costo_enanos_torre_2);
    fprintf(archivo, ELFOS_EXTRA, torres.elfos_extra, configuracion.costo_elfos_torre_1, configuracion.costo_elfos_torre_2);
    fprintf(archivo, ENANOS_ANIMO, juego.fallo_gimli, juego.critico_gimli);
    fprintf(archivo, ELFOS_ANIMO, juego.fallo_legolas, juego.critico_legolas);
    fprintf(archivo, VELOCIDAD_CONFIG, configuracion.velocidad);
    fprintf(archivo, CAMINOS, configuracion.caminos);
    fclose(archivo);
    return !ERROR;
}

/*
 * Definido en configuracion.h
 */
int crear_nueva_configuracion(char config[MAX_ARCHIVO]) {
    configuracion_t configuracion;
    crear_configuracion(&configuracion);
    return escribir_configuracion(configuracion, config);
}

/*
 * Pre: Recibe dos cadenas de caracteres.
 * Pos: Comprueba su coincidencia hasta encontrar un signo FIN_ETIQUETA.
 */
bool tienen_misma_etiqueta(const char etiqueta_1[], const char etiqueta_2[]) {
    bool son_iguales = true, fin_etiqueta = false;
    size_t i = 0, j = 0, tope_1 = strlen(etiqueta_1), tope_2 = strlen(etiqueta_2);
    while (i < tope_1 && j < tope_2 && son_iguales && !fin_etiqueta) {
        if (etiqueta_1[i] == FIN_ETIQUETA)
            fin_etiqueta = true;
        if (etiqueta_1[i] != etiqueta_2[j])
            son_iguales = false;
        i++;
        j++;
    }
    return son_iguales;
}

/*
 * Definido en configuracion.h
 */
int cargar_configuracion(configuracion_t *configuracion, char config[MAX_ARCHIVO]) {
    FILE* archivo;
    inicializar_configuracion(configuracion);
    if (hay_archivo(config)) {
        archivo = fopen(config, LECTURA);
        if (!archivo) {
            printf("Probá con otra configuración.\n");
            return !ERROR;
        }

        char linea[MAX_LINEA];
        fscanf(archivo, "%s", linea);
        while (!feof(archivo)) {
            if (tienen_misma_etiqueta(linea, RESISTENCIA_TORRES)) {
                sscanf(linea, RESISTENCIA_TORRES, &(configuracion->juego.torres.resistencia_torre_1), &(configuracion->juego.torres.resistencia_torre_2));
            } else if (tienen_misma_etiqueta(linea, ENANOS_EXTRA)) {
                sscanf(linea, ENANOS_EXTRA, &(configuracion->juego.torres.enanos_extra), &(configuracion->costo_enanos_torre_1), &(configuracion->costo_enanos_torre_2));
            } else if (tienen_misma_etiqueta(linea, ELFOS_EXTRA)) {
                sscanf(linea, ELFOS_EXTRA, &(configuracion->juego.torres.elfos_extra), &(configuracion->costo_elfos_torre_1), &(configuracion->costo_elfos_torre_2));
            } else if (tienen_misma_etiqueta(linea, ENANOS_INICIO)) {
                sscanf(linea, ENANOS_INICIO, &(configuracion->enanos_inicio[0]), &(configuracion->enanos_inicio[1]), &(configuracion->enanos_inicio[2]), &(configuracion->enanos_inicio[3]));
            } else if (tienen_misma_etiqueta(linea, ELFOS_INICIO)) {
                sscanf(linea, ELFOS_INICIO, &(configuracion->elfos_inicio[0]), &(configuracion->elfos_inicio[1]), &(configuracion->elfos_inicio[2]), &(configuracion->elfos_inicio[3]));
            } else if (tienen_misma_etiqueta(linea, ENANOS_ANIMO)) {
                sscanf(linea, ENANOS_ANIMO, &(configuracion->juego.fallo_gimli), &(configuracion->juego.critico_gimli));
            } else if (tienen_misma_etiqueta(linea, ELFOS_ANIMO)) {
                sscanf(linea, ELFOS_ANIMO, &(configuracion->juego.fallo_legolas), &(configuracion->juego.critico_legolas));
            } else if (tienen_misma_etiqueta(linea, VELOCIDAD_CONFIG)) {
                sscanf(linea, VELOCIDAD_CONFIG, &(configuracion->velocidad));
            } else if (tienen_misma_etiqueta(linea, CAMINOS)) {
                sscanf(linea, CAMINOS, configuracion->caminos);
            }
            fscanf(archivo, "%s", linea);
        }
        fclose(archivo);
    }
    return !ERROR;
}
