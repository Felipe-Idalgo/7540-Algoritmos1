#ifndef __CAMINOS_H__
#define __CAMINOS_H__

void ubicar(coordenada_t *coordenada, int fil, int col);

/*
 *
 */
void crear_camino(int nivel_actual);

/*
 * Recibe un juego inicializando un nuevo nivel. Tomará el valor del
 * nivel actual y buscará en la configuración si hay caminos predefinidos.
 * En caso de que caminos_config se CAMINOS_INDEFINIDOS, se cargarán
 * caminos aleatorios.
 * caminos_config viene del campo caminos de configuracion_t.
 */
void cargar_caminos(juego_t *juego, char caminos_config[MAX_ARCHIVO]);

#endif /*__CAMINOS_H__*/
