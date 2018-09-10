#ifndef __CONEXION_H__
#define __CONEXION_H__
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


typedef struct {
    int self_id;
    int remote_id;
} conexion_t;


int conexion_crear_servidor(conexion_t *conexion, char* puerto, char* host);

int conexion_crear_cliente(conexion_t *conexion, char* puerto, char* host);

int conexion_aceptar_cliente(conexion_t *conexion);

int conexion_enviar_msg(conexion_t *conexion, void* msg, int32_t tamanio);

int conexion_recibir_msg(conexion_t *conexion, void* mensaje);

int conexion_recibir_msg2(conexion_t *conexion, void** mensaje);

void conexion_destruir(conexion_t *conexion);

#endif
