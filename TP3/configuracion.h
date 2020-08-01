#ifndef __CONFIGURACION_H__
#define __CONFIGURACION_H__
#include <stdbool.h>

/*
 *
 */
int crear_nueva_configuracion(char config[MAX_ARCHIVO]);

/*
 * Pre: Recibe el nombre de un archivo de configuración que se desee leer o ninguno.
 * Pos: Inicializará la configuación con los datos que se hayan podido extraer del archivo.
 *      Los valores que no se hayan podido extraer del archivo, se inicializarán con los
 *      predeterminados.
 */
int cargar_configuracion(configuracion_t *configuracion, char config[MAX_ARCHIVO]);

bool tienen_misma_etiqueta(const char etiqueta_1[], const char etiqueta_2[]);

#endif /*__CONFIGURACION_H__*/
