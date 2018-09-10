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

//#define OK 0
//#define ERROR 1
static const int OK = 0;
static const int ERROR = 1;

int conexion_crear_servidor(conexion_t *conexion, char* puerto, char* host) {
    struct addrinfo hints;
    struct addrinfo *ptr;
    int32_t status;
    int32_t val;

    memset(&hints, 0, sizeof(struct addrinfo));
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

    status = listen(conexion->self_id, 20);
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

    memset(&hints, 0, sizeof(struct addrinfo));
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
        shutdown(conexion->remote_id , SHUT_RDWR);
        close(conexion->remote_id );
        return ERROR;
    }
    return OK;
}

int conexion_enviar_msg(conexion_t *conexion, void* msg, int32_t tamanio) {
    int32_t longitud_enviar = htonl(tamanio);
    int32_t total_enviados = 0;
    int32_t enviado;
    char* buffer = (char*) msg;
    bool socket_error = false;

    enviado = send(conexion->remote_id, &longitud_enviar,
                   sizeof(int32_t), MSG_NOSIGNAL);

    if (enviado < sizeof(int32_t)) {
        socket_error = true;
        fprintf(stderr, "ERROR DE LONGITUD\n");
    }

    while (total_enviados < tamanio && socket_error == false) {
        enviado = send(conexion->remote_id, buffer,
                       tamanio - total_enviados, MSG_NOSIGNAL);

        if (enviado <= 0) {
            socket_error = true;
            fprintf(stderr, "ERROR AL ENVIAR EL MENSAJE\n");
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

    return OK;
}

int recivir_mensaje(conexion_t *conexion, void** msg){
    bool valido = true;
    bool abierto = true;
    int32_t longitud_mensaje = 0;
    int32_t total_recivido = 0;
    int32_t recivido = 0;

    recivido = recv(conexion->remote_id, &longitud_mensaje,
                    sizeof(int32_t), MSG_NOSIGNAL);
    if (recivido < sizeof(int32_t)) {
        valido = false;
        fprintf(stderr, "ERROR DE LONGITUD\n");
    }

    longitud_mensaje = ntohl(longitud_mensaje);
    char* buffer = malloc(sizeof(char) * longitud_mensaje);
    *msg = buffer;

    while (total_recivido < longitud_mensaje && valido && abierto) {
        recivido = recv(conexion->remote_id, buffer,
                        longitud_mensaje - total_recivido, MSG_NOSIGNAL);

        if (recivido < 0) {
            valido = false;
            fprintf(stderr, "ERROR AL RECIVIR EL MENSAJE\n");
        } else if (recivido == 0) {
            abierto = false;
        } else {
            total_recivido += recivido;
            buffer += recivido;
        }
    }

    if (valido) {
        return total_recivido;
    } else {
        free(buffer);
        return -1;
    }
}

int conexion_recibir_msg(conexion_t *conexion, void* mensaje) {
    void* msg;
    int32_t total_recivido;


    total_recivido = recivir_mensaje(conexion, &msg);
    if (total_recivido != -1) {
        memcpy(mensaje, msg, total_recivido);
        free(msg);
        return total_recivido;
    } else {
        return -1;
    }
}

int conexion_recibir_msg2(conexion_t *conexion, void** mensaje){
    void* msg;
    int32_t total_recivido;

    total_recivido = recivir_mensaje(conexion, &msg);
    if (total_recivido != -1) {
        *mensaje = malloc(total_recivido);
        memcpy(*mensaje, msg, total_recivido);
        free(msg);
        return total_recivido;
    } else {
        return -1;
    }
}

void conexion_destruir(conexion_t *conexion) {
    shutdown(conexion->remote_id, SHUT_RDWR);
    close(conexion->remote_id);
}
