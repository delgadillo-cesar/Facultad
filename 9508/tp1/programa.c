#include "programa.h"
#include <string.h>
#include <stdio.h>

//#define OK 0
//#define ERROR 1
static const int OK = 0;
static const int ERROR = 1;

int programa_descarcar_bytecodes(programa_t *programa);

int programa_crear_remoto(programa_t *programa, conexion_t* conexion,
                          size_t longitud){
    programa->programa = malloc(longitud);
    programa->tamanio_programa = longitud;
    programa->instruccion_actual = 0;
    programa->conexion = conexion;
    programa->ultima_descargada = -1;

    return OK;
}

int programa_obtener_instruccion(programa_t *programa, char *instruccion){
    int status;

    if (programa->instruccion_actual >= programa->tamanio_programa)
        return ERROR;

    if (programa->instruccion_actual > programa->ultima_descargada) {
        status = programa_descarcar_bytecodes(programa);
        if (status == ERROR) return ERROR;
    }

    char *origen = (programa->programa);
    origen += (programa->instruccion_actual);
    memcpy(instruccion, origen, sizeof(char));
    programa->instruccion_actual++;

    return OK;
}

int programa_destruir(programa_t *programa){
    free(programa->programa);

    return OK;
}

int programa_descarcar_bytecodes(programa_t *programa) {
    void *buffer;
    int status;
    char* msg = "mas_datos";
    uint32_t size_msg = sizeof(char*) + strlen(msg) + 1;

    status = conexion_enviar_msg(programa->conexion, msg, size_msg);
    if (status == ERROR) {
        return ERROR;
    }

    status = conexion_recibir_msg2(programa->conexion, &buffer);
    if (status == -1) {
        return ERROR;
    }

    char *bytecodes = (programa->programa);
    bytecodes += (programa->ultima_descargada + 1);
    memcpy(bytecodes, buffer, status);
    free(buffer);

    programa->ultima_descargada += status;

    return OK;
}
