#ifndef __JUEGO_H__
#define __JUEGO_H__

#define MAX_NOMBRE 20

typedef struct rank {
    char nombre[MAX_NOMBRE];
    int puntaje;
} rank_t;

/*
 * Se utilizará una configuración con valores predeterminados o personalizados.
 * Se podrá grabar la partida si se tienen los permisos suficientes.
 * El rank se guardará si se tienen los permisos suficientes.
 * > Devuelve -1 si no se pudo utilizar la configuración o no se pudo grabar.
 * > Devuelve 0 si no hubo problemas.
 */
int iniciar_juego(configuracion_t configuracion, char grabacion[MAX_ARCHIVO], rank_t* rank);

/*
 * Se ingresa la grabación de una partida. Se reproducirá el juego
 * en sus diferentes estadios sin interrupciones hasta finalizar.
 * Sólo será posible ver los movimientos y los estados del juego.
 * No se contará el puntaje. La velocidad es opciona, es el tiempo de
 * espera entre turnos.
 * > Devuelve -1 si no se pudo abrir el archivo.
 * > Devuelve 0 si no hubo problemas.
 */
int reproducir_juego(char grabacion[MAX_ARCHIVO], float velocidad);

#endif /*__JUEGO_H__*/
