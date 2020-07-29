#ifndef __CONFIGURACION_H__
#define __CONFIGURACION_H__

/*
 * Muestra en pantalla todos los mensajes para que el usuario pueda crear
 * una configuración nueva con valores válidos.
 */
void crear_configuracion(configuracion_t *configuracion);

/*
 * Pre: Recibe la configuración que se desee guardar y el nombre del archivo que se utilizará
 * Pos: Si el archivo no existe se creará. Si existe se sobreescribirá. En él se guardarán
 *     los datos de la configuración recibida.
 */
void escribir_configuracion(configuracion_t configuraciont, char config[MAX_ARCHIVO]);

/*
 * Pre: Recibe el nombre de un archivo de configuración que se desee leer o ninguno.
 * Pos: Inicializará la configuación con los datos que se hayan podido extraer del archivo.
 *      Los valores que no se hayan podido extraer del archivo, se inicializarán con los
 *      predeterminados.
 */
void cargar_configuracion(configuracion_t *configuracion, char config[MAX_ARCHIVO]);

#endif /*__CONFIGURACION_H__*/
