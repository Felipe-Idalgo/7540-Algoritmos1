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
void iniciar_juego(configuracion_t configuracion, FILE* grabacion, rank_t* rank);


/*
 * Se mostrará en pantalla la grabación de un juego en la velocidad deseada.
 */
void reproducir(FILE* grabacion, float velocidad);

#endif /*__JUEGO_H__*/
