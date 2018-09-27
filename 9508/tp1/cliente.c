#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conexion.h"
#include "cliente.h"

static const int OK = 0;
static const int ERROR = 1;
static const int ERROR_CONEX = -1;
static const int BUFFER_LECTURA = 5;

int leer_archivo(FILE* archivo, void** buffer, int cantidad) {
    char* leido = malloc(sizeof(char) * cantidad);
    int idx = 0;
    char dato;

    while ((!feof(archivo)) && (idx < cantidad)) {
        dato = fgetc(archivo);
        leido[idx] = dato;
        idx++;
    }

    if (idx > 0) {
        memcpy(*buffer, leido, idx);
    }
    free(leido);

    return idx;
}

int resultados(conexion_t* conexion, int32_t mem_size) {
    uint32_t len_memoria = sizeof(uint32_t*) * mem_size;
    void* mensaje_respuesta = malloc(len_memoria);
    int status;
    int32_t* memoria;

    status = conexion_recibir_msg(conexion, mensaje_respuesta, len_memoria);
    if (status <= 0) {
        fprintf(stderr, "Error al recibir dump de variables\n");
        return ERROR;
    }

    memoria = (int32_t *) mensaje_respuesta;
    fprintf(stdout, "%s\n", "Variables dump");

    for (int i = 0; i < mem_size; i++) {
	fprintf(stdout, "%08x\n", ntohl(memoria[i]));
    }

    free(mensaje_respuesta);
    return OK;
}

int iniciar_cliente(char* host, char* port, int32_t memoria, FILE* archivo) {
    int32_t mem = htonl(memoria);
    bool terminar = false;
    conexion_t conexion;

    int status = conexion_crear_cliente(&conexion,port, host);
    if (status == ERROR_CONEX) {
        fprintf(stderr, "No se pudo establecer la conexion con el servidor\n");
        return ERROR;
    }

    status = conexion_enviar_msg(&conexion, &mem, sizeof(mem));
    if (status == ERROR_CONEX) {
        fprintf(stderr, "Error al enviar el mensaje\n");
        return ERROR;
    }

    void* enviar = malloc(sizeof(char) * BUFFER_LECTURA);
    int cant_enviar = 0;
    uint32_t size_enviar = 0;

    while (!terminar) {
        cant_enviar = leer_archivo(archivo, &enviar, BUFFER_LECTURA);

        if (cant_enviar == 0) {
            terminar = true;
        } else {
            size_enviar = sizeof(char) * cant_enviar;
            status = conexion_enviar_msg(&conexion, enviar, size_enviar);
            if (status == ERROR_CONEX) {
                free(enviar);
                conexion_finalizar(&conexion);
                return ERROR;
            } else if (status != size_enviar) {
                free(enviar);
                conexion_finalizar(&conexion);
                return ERROR;
            }
        }
    }
    free(enviar);

    conexion_cerrar_escritura(&conexion);

    if (resultados(&conexion, memoria) == ERROR) {
        conexion_finalizar(&conexion);
        return ERROR;
    }

    conexion_finalizar(&conexion);

    return OK;
}
