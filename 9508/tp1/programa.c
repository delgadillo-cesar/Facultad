#include "programa.h"
#include <string.h>
#include <stdio.h>
#define OK 0
#define ERROR 1

int programa_descarcar_bytecodes(programa_t *un_programa);

int programa_crear(programa_t *un_programa, void *byte_codes, size_t longitud){
    un_programa->programa = malloc(longitud);
    memcpy(un_programa->programa, byte_codes, longitud);
    un_programa->tamanio_programa = longitud;
    un_programa->instruccion_actual = 0;
    un_programa->conexion = NULL;
    un_programa->ultima_instruccion_descargada = longitud - 1;

    return OK;
}

int programa_crear_remoto(programa_t *un_programa, conexion_t* conexion, size_t longitud){
    un_programa->programa = malloc(longitud);
    un_programa->tamanio_programa = longitud;
    un_programa->instruccion_actual = 0;
    un_programa->conexion = conexion;
    un_programa->ultima_instruccion_descargada = -1;

    return OK;
}

int programa_obtener_instruccion(programa_t *un_programa, char *instruccion){
    int status;

    if (un_programa->instruccion_actual >= un_programa->tamanio_programa)
        return ERROR;

    if ( un_programa->instruccion_actual > un_programa->ultima_instruccion_descargada) {
        status = programa_descarcar_bytecodes(un_programa);
        if (status == ERROR) return ERROR;
    }

    char *origen = (un_programa->programa);
    origen += (un_programa->instruccion_actual);
    memcpy(instruccion, origen, sizeof(char));
    un_programa->instruccion_actual++;

    return OK;
}

int programa_destruir(programa_t *un_programa){
    free(un_programa->programa);

    return OK;
}


int programa_descarcar_bytecodes(programa_t *un_programa) {
    void *buffer;
    int status;
    char* msg = "mas_datos";

    status = conexion_enviar_mensaje(un_programa->conexion, msg, sizeof(char*) + strlen(msg) + 1);
    if (status == ERROR) {
        return ERROR;
    }

    status = conexion_recibir_mensaje2(un_programa->conexion, &buffer);
    if (status == -1) {
        return ERROR;
    }

    char *bytecodes = (un_programa->programa);
    bytecodes += (un_programa->ultima_instruccion_descargada + 1);
    memcpy(bytecodes, buffer, status);
    free(buffer);

    un_programa->ultima_instruccion_descargada += status;

    return OK;
}
