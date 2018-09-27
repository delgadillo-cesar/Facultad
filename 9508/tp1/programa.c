#include "programa.h"
#include <string.h>
#include <stdio.h>

static const int OK = 0;
static const int ERROR = 1;
static const int CANT_RECIVIR = 10;

int programa_descarcar_bytecodes(programa_t *programa);

int programa_crear_remoto(programa_t *programa, conexion_t* conexion) {
    programa->programa = NULL;
    programa->instruccion_actual = 0;
    programa->conexion = conexion;
    programa->ultima_descargada = -1;

    return OK;
}

int programa_obtener_instruccion(programa_t *programa, uint8_t *instruccion) {
    int status;

    if (programa->instruccion_actual >= programa->ultima_descargada) {
        status = programa_descarcar_bytecodes(programa);
        if (status == ERROR) return ERROR;
    }

    if (programa->instruccion_actual >= programa->ultima_descargada)
        return ERROR;

    char *origen = (programa->programa);
    origen += (programa->instruccion_actual);
    memcpy(instruccion, origen, sizeof(char));
    programa->instruccion_actual++;

    return OK;
}

int programa_finalizar(programa_t *programa){
    free(programa->programa);

    return OK;
}

int programa_descarcar_bytecodes(programa_t *programa) {
    void *buffer = malloc(sizeof(char) * CANT_RECIVIR);
    int status;

    status = conexion_recibir_msg(programa->conexion, buffer, CANT_RECIVIR);
    if (status == -1) {
        free(buffer);
        return ERROR;
    }

    int long_prog = programa->ultima_descargada + 1 + status;
    programa->programa = realloc(programa->programa, long_prog);
    char *bytecodes = (programa->programa);
    bytecodes += (programa->ultima_descargada + 1);
    memcpy(bytecodes, buffer, status);
    free(buffer);

    programa->ultima_descargada += status;

    return OK;
}
