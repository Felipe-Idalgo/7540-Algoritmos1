#ifndef __CONFIGURACION_H__
#define __CONFIGURACION_H__

#include <stdbool.h>
#include "archivos.h"

#define MAX_NIVELES 4
#define INDEFINIDO -1

typedef struct configuracion {
    juego_t juego;
    int enanos_inicio[MAX_NIVELES];
    int elfos_inicio[MAX_NIVELES];
    int costo_enanos_torre_1;
    int costo_enanos_torre_2;
    int costo_elfos_torre_1;
    int costo_elfos_torre_2;
    float velocidad;
    char caminos[MAX_ARCHIVO];
} configuracion_t;

/*
 * Le da al usuario las opciones para crear una nueva configuración.
 * Debe recibir el nombre de un archivo para crear o sobreescribir
 * un archivo bajo esa ruta.
 * Devuelve 0 si se pudo crear la configuracion.
 * Devuelve ERROR si hubo problemas.
 */
int crear_nueva_configuracion(char config[MAX_ARCHIVO]);

/*
 * Pre: Recibe el nombre de un archivo de configuración que se desee leer o ninguno.
 * Pos: Inicializará la configuación con los datos que se hayan podido extraer del archivo.
 *      Los valores que no se hayan podido extraer del archivo, se inicializarán con los
 *      predeterminados.
 *      Devolverá 0 si se pudo cargar la configuración.
 */
int cargar_configuracion(configuracion_t *configuracion, char config[MAX_ARCHIVO]);

/*
 * Pre: Recibe dos cadenas de caracteres.
 * Pos: Comprueba su coincidencia hasta encontrar un signo FIN_ETIQUETA.
 */
bool tienen_misma_etiqueta(const char etiqueta_1[], const char etiqueta_2[]);

#endif /*__CONFIGURACION_H__*/
