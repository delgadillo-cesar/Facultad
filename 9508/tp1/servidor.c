#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "programa.h"
#include "conexion.h"
static const int ERROR_CONEX = -1;

static const int OK = 0;
static const int ERROR = 1;


int variables_dump(conexion_t* conexion, int* memoria, int32_t mem_size) {
    int status;
    char titulo[] = "Variables dump";
    int32_t size_resultados;
    int32_t* memoria_send = malloc(sizeof(int32_t) * mem_size);


    fprintf(stdout, "\n%s\n", titulo);

    for (int i = 0; i < mem_size; i++) {
        memoria_send[i] = htonl(memoria[i]);
        fprintf(stdout, "%08x\n", memoria[i]);
    }

    size_resultados = sizeof(int32_t) * mem_size;
    status = conexion_enviar_msg(conexion, memoria_send, size_resultados);
    if (status == ERROR_CONEX) {
        return ERROR;
    }

    free(memoria_send);

    return OK;
}

int preparar_servidor(conexion_t* conexion, char* port) {
    if (conexion_crear_servidor(conexion, port, "localhost") == ERROR) {
        fprintf(stderr, "No se pudo iniciar la conexion\n");
        return ERROR;
    }

    if (conexion_aceptar_cliente(conexion) == ERROR) {
        fprintf(stderr, "Error al conectar al cliente\n");
        conexion_finalizar(conexion);
        return ERROR;
    }

    return OK;
}

int iniciar_servidor(char* port) {
    conexion_t servidor;
    programa_t programa;
    jvm_t jvm;
    int32_t cant_mem;
    int* memoria;

    if (preparar_servidor(&servidor, port) == ERROR)
        return ERROR;

    if (conexion_recibir_msg(&servidor, &cant_mem, sizeof(int32_t)) == -1) {
        fprintf(stderr, "Error al recivir la cantidad de memoria\n");
        conexion_finalizar(&servidor);
        return ERROR;
    } else {
        cant_mem = ntohl(cant_mem);
    }

    memoria = malloc(sizeof(int) * cant_mem);
    memset(memoria, 0, sizeof(int) * cant_mem);
    jvm_crear(&jvm, memoria, cant_mem);

    programa_crear_remoto(&programa, &servidor);
    jvm_procesar(&jvm, &programa);
    variables_dump(&servidor, memoria, cant_mem);

    jvm_finalizar(&jvm);
    free(memoria);
    programa_finalizar(&programa);
    conexion_finalizar(&servidor);

    return OK;
}
