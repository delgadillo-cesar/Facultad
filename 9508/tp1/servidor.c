#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "programa.h"
#include "conexion.h"

#define OK 0
#define ERROR 1


int variables_dump(conexion_t* conexion, int* memoria, int32_t mem_size) {
    int idx = 0, status;
    char titulo[] = "Variables dump\n";
    char* msg = "resultados";
    char* salida;
    int32_t size_resultados;
    int32_t size_salida;


    size_salida = sizeof(char) *  (strlen(titulo) + (9 * mem_size) + 1);
    salida = malloc(size_salida);
    sprintf(salida, "%s", titulo);
    idx += strlen(titulo);

    for (int i = 0; i < mem_size; i++) {
        sprintf(salida+idx, "%08x\n", memoria[i]);
        idx += 9;
    }
    salida[idx] = 0;

    size_resultados = sizeof(char) * strlen(msg) + 1;
    status = conexion_enviar_msg(conexion, msg, size_resultados);
    if (status == ERROR) {
        return ERROR;
    }

    status = conexion_enviar_msg(conexion, salida, size_salida);
    if (status == ERROR) {
        return ERROR;
    }
    fprintf(stdout, "\n%s", salida);

    free(salida);
    return OK;
}

int iniciar_servidor(char* port) {
    conexion_t servidor;
    programa_t programa;
    jvm_t jvm;
    int32_t long_prog;
    int32_t cant_mem;
    int* memoria;

    if (conexion_crear_servidor(&servidor, port, "localhost") == ERROR) {
        fprintf(stderr, "No se pudo iniciar la conexion\n");
        return ERROR;
    }

    if (conexion_aceptar_cliente(&servidor) == ERROR) {
        fprintf(stderr, "Error al conectar al cliente\n");
        return ERROR;
    }

    if (conexion_recibir_msg(&servidor, &cant_mem) == -1) {
        fprintf(stderr, "Error al recivir la cantidad de memoria\n");
        return ERROR;
    } else {
        cant_mem = ntohl(cant_mem);
    }

    if (conexion_recibir_msg(&servidor, &long_prog) == -1) {
        fprintf(stderr, "Error al recivir la longitud del programa\n");
        return ERROR;
    } else {
        long_prog = ntohl(long_prog);
    }

    if (long_prog <= 0) {
        fprintf(stderr, "Longitud del programa cero\n");
        return ERROR;
    }

    memoria = malloc(sizeof(int*) * cant_mem);
    jvm_crear(&jvm, memoria, cant_mem);

    programa_crear_remoto(&programa, &servidor, long_prog);
    jvm_procesar(&jvm, &programa);
    variables_dump(&servidor, memoria, cant_mem);

    jvm_finalizar(&jvm);
    free(memoria);
    programa_destruir(&programa);
    conexion_destruir(&servidor);

    return OK;
}
