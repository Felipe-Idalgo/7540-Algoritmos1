#ifndef __CONFIGURACION_H__
#define __CONFIGURACION_H__

void crear_configuracion(configuracion_t *configuracion);

void escribir_configuracion(configuracion_t configuraciont, char config[MAX_ARCHIVO]);

void cargar_configuracion(configuracion_t *configuracion, char config[MAX_ARCHIVO]);

#endif /*__CONFIGURACION_H__*/
