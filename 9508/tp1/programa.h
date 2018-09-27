#ifndef __PROGRAMA_H__
#define __PROGRAMA_H__
#include <stdlib.h>
#include "conexion.h"

typedef struct {
    char *programa;
    int instruccion_actual;
    conexion_t *conexion;
    int ultima_descargada;
} programa_t;

int programa_crear_remoto(programa_t *programa, conexion_t* conexion);

int programa_obtener_instruccion(programa_t *programa, uint8_t *instruccion);

int programa_finalizar(programa_t *programa);

#endif
