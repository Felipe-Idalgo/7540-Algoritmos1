#ifndef __JUEGO_H__
#define __JUEGO_H__

#define MAX_NOMBRE 100

typedef struct rank {
    char nombre[MAX_NOMBRE];
    int puntaje;
} rank_t;

/*
 * Se utilizará una configuración con valores predeterminados o personalizados.
 * Se podrá grabar la partida si se tienen los permisos suficientes.
 * El rank se guardará si se tienen los permisos suficientes.
 */
void iniciar_juego(configuracion_t configuracion, char grabacion[], rank_t* rank);

/*
 * Se ingresa la grabación de una partida. Se reproducirá el juego
 * en sus diferentes estadios sin interrupciones hasta finalizar.
 * Sólo será posible ver los movimientos y los estados del juego.
 * No se contará el puntaje. La velocidad es opciona, es el tiempo de
 * espera entre turnos.
 */
void reproducir_juego(char grabacion[], float velocidad);

#endif /*__JUEGO_H__*/
