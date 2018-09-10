#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conexion.h"
#include "servidor.h"
#include "cliente.h"

#define OK 0
#define ERROR 1

int leer_archivo(FILE* archivo, int longitud, void** buffer, int cantidad) {
    char* leido = malloc(sizeof(char) * cantidad);
    int idx = 0;
    char dato;

    while ((longitud > ftell(archivo)) && (idx < cantidad)) {
        dato = fgetc(archivo);
        leido[idx] = dato;
        idx++;
    }

    if (idx > 0) {
        *buffer = malloc(sizeof(char) * idx);
        memcpy(*buffer, leido, idx);
    }
    free(leido);

    return idx;
}

int mas_datos(conexion_t* conexion, FILE* archivo, int longitud) {
    void* enviar;
    int cant_enviar;
    int status;
    uint32_t size_enviar;

    cant_enviar = leer_archivo(archivo, longitud, &enviar, 5);

    if (cant_enviar > 0) {
        size_enviar = sizeof(char) * cant_enviar;
        status = conexion_enviar_msg(conexion, enviar, size_enviar);
        if (status != OK) {
            free(enviar);
            fprintf(stderr, "Error al enviar el mensaje\n");
            return ERROR;
        }
    free(enviar);
    }

    return OK;
}

int resultados(conexion_t* conexion) {
    void* mensaje_respuesta = NULL;
    int status;

    status = conexion_recibir_msg2(conexion, &mensaje_respuesta);
    if (status <= 0) {
        fprintf(stderr, "Error al recibir dump de variables\n");
        return ERROR;
    }
    fprintf(stdout, "%s\n", (char*) mensaje_respuesta);
    free(mensaje_respuesta);

    return OK;
}

int iniciar_cliente(char* host, char* port, int32_t memoria, FILE* archivo) {
    int status;
    void* mensaje_respuesta = NULL;
    int32_t long_prog;
    int32_t mem = htonl(memoria);
    bool terminar = false;
    conexion_t conexion;
    conexion_crear_cliente(&conexion,port, host);

    status = conexion_enviar_msg(&conexion, &mem, sizeof(mem));
    if (status != OK) {
        fprintf(stderr, "Error al enviar el mensaje\n");
        return ERROR;
    }

    fseek(archivo, 0, SEEK_END);
    long_prog = htonl(ftell(archivo));
    fseek(archivo, 0, SEEK_SET);

    status = conexion_enviar_msg(&conexion, &long_prog, sizeof(long_prog));
    if (status != OK) {
        fprintf(stderr, "Error al enviar el mensaje\n");
        return ERROR;
    }
    long_prog = ntohl(long_prog);
    if (long_prog <= 0) {
        fprintf(stderr, "Longitud del programa cero\n");
        return ERROR;
    }

    while (!terminar) {
        status = conexion_recibir_msg2(&conexion, &mensaje_respuesta);
        if (status <= 0) {
            fprintf(stderr, "Error al recibir respuesta - instruccion\n");
            return ERROR;
        }

        if (!strcmp(mensaje_respuesta, "mas_datos")) {
            free(mensaje_respuesta);
            if (mas_datos(&conexion, archivo, long_prog) == ERROR) {
                return ERROR;
            }
        }else if (!strcmp(mensaje_respuesta, "resultados")) {
            terminar = true;
            free(mensaje_respuesta);
            if (resultados(&conexion) == ERROR) {
                return ERROR;
            }
        }
    }

    conexion_destruir(&conexion);

    return OK;
}
