#define _POSIX_C_SOURCE 200112L

#include "conexion.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


static const int OK = 0;
static const int ERROR = -1;

int conexion_crear_servidor(conexion_t *conexion, char* puerto, char* host) {
    struct addrinfo hints;
    struct addrinfo *ptr;
    int32_t status;
    int32_t val;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(host, puerto, &hints, &ptr);
    if (status != 0) {
        fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(status));
        return ERROR;
    }


    conexion->self_id = socket(ptr->ai_family,
                               ptr->ai_socktype,
                               ptr->ai_protocol);
    if (conexion->self_id == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        freeaddrinfo(ptr);
        return ERROR;
    }

    val = 1;
    status = setsockopt(conexion->self_id,
                        SOL_SOCKET,
                        SO_REUSEADDR,
                        &val,
                        sizeof(val));

    if (status == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        close(conexion->self_id);
        freeaddrinfo(ptr);
        return ERROR;
    }

    status = bind(conexion->self_id, ptr->ai_addr, ptr->ai_addrlen);
    if (status == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        close(conexion->self_id);
        freeaddrinfo(ptr);
        return ERROR;
    }

    freeaddrinfo(ptr);

    status = listen(conexion->self_id, 1);
    if (status == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        close(conexion->self_id);
        return ERROR;
    }

    return OK;
}

int conexion_crear_cliente(conexion_t *conexion, char* puerto, char* host) {
    struct addrinfo hints;
    struct addrinfo *ptr;
    struct addrinfo *lista;
    int32_t status;
    bool conectado = false;

    conexion->self_id = -1;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    status = getaddrinfo(host, puerto, &hints, &lista);
    if (status != 0) {
        fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(status));
        return ERROR;
    }

    for (ptr = lista; ptr != NULL && !conectado; ptr = ptr->ai_next) {
        conexion->remote_id = socket(ptr->ai_family,
                                     ptr->ai_socktype,
                                     ptr->ai_protocol);

        if (conexion->remote_id == -1) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
        } else {
            status = connect(conexion->remote_id,
                             ptr->ai_addr,
                             ptr->ai_addrlen);

            if (status == -1) {
                fprintf(stderr,"Error: %s\n", strerror(errno));
                close(conexion->remote_id);
            }
            conectado = (status != -1);
        }
    }

    freeaddrinfo(lista);

    if (!conectado) {
        return ERROR;
    }
    return OK;
}

int conexion_aceptar_cliente(conexion_t *conexion) {
    conexion->remote_id = accept(conexion->self_id, NULL, NULL);
    if (conexion->remote_id  == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        close(conexion->remote_id);
        return ERROR;
    }
    return OK;
}

int conexion_enviar_msg(conexion_t *conexion, void* msg, int32_t tamanio) {
    int32_t total_enviados = 0;
    int32_t enviado = 0;
    char* buffer = (char*) msg;
    bool socket_cerrado = false;
    bool socket_error = false;

    while (total_enviados < tamanio && !socket_error && !socket_cerrado) {
        enviado = send(conexion->remote_id, buffer,
                       tamanio - total_enviados, MSG_NOSIGNAL);

        if (enviado < 0) {
            socket_error = true;
            fprintf(stderr, "ERROR AL ENVIAR EL MENSAJE\n");
        } else if (enviado == 0){
            socket_cerrado = true;
        } else {
            total_enviados += enviado;
            buffer += total_enviados;
       }
    }

    if (socket_error) {
        shutdown(conexion->remote_id, SHUT_RDWR);
        close(conexion->remote_id);
        return ERROR;
    }

    return total_enviados;
}

int conexion_recibir_msg(conexion_t *conexion, void* buffer, int buffer_size) {
    bool socket_cerrado = false;
    bool socket_error = false;

    int32_t total_recivido = 0;
    int32_t recivido = 0;
    char* s_buffer = (char*) buffer;

    while (total_recivido < buffer_size && !socket_error && !socket_cerrado) {
        recivido = recv(conexion->remote_id, s_buffer,
                        buffer_size - total_recivido, MSG_NOSIGNAL);

        if (recivido < 0) {
            socket_error = true;
            fprintf(stderr, "ERROR AL RECIVIR EL MENSAJE\n");
        } else if (recivido == 0) {
            socket_cerrado = true;
        } else {
            total_recivido += recivido;
            s_buffer += recivido;
        }
    }

    if (!socket_error) {
        return total_recivido;
    } else {
        return -1;
    }
}

int conexion_cerrar_lectura(conexion_t *conexion) {
    return shutdown(conexion->remote_id, SHUT_RD);
}

int conexion_cerrar_escritura(conexion_t *conexion) {
    return shutdown(conexion->remote_id, SHUT_WR);
}

void conexion_finalizar(conexion_t *conexion) {
    shutdown(conexion->remote_id, SHUT_RDWR);
    close(conexion->remote_id);

    if (conexion->self_id > 0) {
        shutdown(conexion->self_id, SHUT_RDWR);
        close(conexion->self_id);
    }
}
