#ifndef __CAMINOS_H__
#define __CAMINOS_H__

#include "archivos.h"

#define MINIMO_MATRIZ 0
#define MAXIMO_MATRIZ_MENOR 14
#define MAXIMO_MATRIZ_MAYOR 19

//~ Pre: Recibe un numero de fila y un numero de columna para una matriz.
//~ Pos: Inicializa una coordenada.
void ubicar(coordenada_t *coordenada, int fil, int col);

/*
 * Pre: Recibe el nombre del archivo de caminos que va a crearse o sobreescribirse.
 * Pos: Permite al usuario crear los caminos por cada nive y crea el archivo
 *      con sus caminos.
 *      Devuelve ERROR si no se pudo grabar.
 */
int crear_nuevos_caminos(char caminos[MAX_ARCHIVO]);

/*
 * Recibe un juego inicializando un nuevo nivel. Tomará el valor del
 * nivel actual y buscará en la configuración si hay caminos predefinidos.
 * En caso de que caminos_config sea CAMINOS_INDEFINIDOS, se cargarán
 * caminos aleatorios.
 * caminos_config viene del campo caminos de configuracion_t.
 * > Devuelve 0 si pueden cargar caminos.
 * > Devuelve ERROR si hubo un problema con la lectura del archivo.
 */
int cargar_caminos(juego_t *juego, char caminos_config[MAX_ARCHIVO]);

#endif /*__CAMINOS_H__*/
