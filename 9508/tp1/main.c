#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
#include "jvm.h"
#include "programa.h"
#include "conexion.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define OK 0
#define ERROR 1


/*\x10\x41\x59\x59\x36\x00\x10\x01\x60\x59\x59\x36\x01*/

int variables_dump(conexion_t* conexion, int* memoria, size_t tamanio_memoria) {
    int idx = 0, status;
    char titulo[] = "Variables dump\n";
    uint32_t len_salida = strlen(titulo) + (8 * (tamanio_memoria + 1)) + 1;
    char* salida = malloc(sizeof(char) * len_salida);
    sprintf(salida, "%s", titulo);
    idx += strlen(titulo);

    for (int i = 0; i < tamanio_memoria; i++) {
        sprintf(salida+idx, "%08X\n", memoria[i]);
        idx += 9;
    }
    salida[idx] = 0;


    char* msg = "resultados";
    status = conexion_enviar_mensaje(conexion, msg, sizeof(char) * strlen(msg) + 1);
    if (status == ERROR) {
        return ERROR;
    }

    status = conexion_enviar_mensaje(conexion, salida, sizeof(char) * (strlen(salida) + 1));
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
    uint32_t long_prog;
    uint32_t cant_mem;
    int* memoria;

    if (conexion_crear_servidor(&servidor, port, "localhost") == ERROR) {
        fprintf(stderr, "No se pudo iniciar la conexion\n");
        return ERROR;
    }

    if (conexion_aceptar_cliente(&servidor) == ERROR) {
        fprintf(stderr, "Error al conectar al cliente\n");
        return ERROR;
    }

    if (conexion_recibir_mensaje(&servidor, &cant_mem) == -1) {
        fprintf(stderr, "Error al recivir la cantidad de memoria\n");
        return ERROR;
    } else {
        cant_mem = ntohl(cant_mem);
    }

    if (conexion_recibir_mensaje(&servidor, &long_prog) == -1) {
        fprintf(stderr, "Error al recivir la longitud del programa\n");
        return ERROR;
    } else {
        long_prog = ntohl(long_prog);
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


int leer_archivo(FILE* archivo, void** buffer, int cantidad) {
    char dato;
    char* leido = malloc(sizeof(char) * cantidad);
    int idx = 0;

    do {
        dato = fgetc(archivo);
        if (dato != EOF) {
            leido[idx] = dato;
            idx++;
        }
    } while ((dato != EOF) && idx < cantidad);

    if (idx > 0) {
        *buffer = malloc(sizeof(char) * idx);
        memcpy(*buffer, leido, idx);
    }
    free(leido);

    return idx;
}


        /**./tp client ​ <host> <port>​ ​ <N> ​ [ ​ <filename>​ ]*/
int iniciar_cliente(char* host, char* port, uint32_t memoria, FILE* archivo) {
    int status;
    int cant_enviar;
    void* mensaje_respuesta = NULL;
    void* enviar;
    uint32_t long_prog;
    uint32_t mem = htonl(memoria);
    bool terminar = false;
    conexion_t conexion;
    conexion_crear_cliente(&conexion,port, host);

    status = conexion_enviar_mensaje(&conexion, &mem, sizeof(mem));
    if (status != OK) {
        fprintf(stderr, "Error al enviar el mensaje\n");
        return ERROR;
    }

    fseek(archivo, 0, SEEK_END);
    long_prog = htonl(ftell(archivo));
    fseek(archivo, 0, SEEK_SET);

    status = conexion_enviar_mensaje(&conexion, &long_prog, sizeof(long_prog));
    if (status != OK) {
        fprintf(stderr, "Error al enviar el mensaje\n");
        return ERROR;
    }

    while (!terminar) {
        status = conexion_recibir_mensaje2(&conexion, &mensaje_respuesta);
        if (status <= 0) {
            fprintf(stderr, "Error al recibir respuesta\n");
            return ERROR;
        }

        if (!strcmp(mensaje_respuesta, "mas_datos")) {
            free(mensaje_respuesta);
            cant_enviar = leer_archivo(archivo, &enviar, 5);
/*
            printf("LEIDO: ");
            for (int i = 0; i <cant_enviar; i++) {
                printf(" %X", ((char*)enviar)[i]);
            }
            printf("\n");
*/
            if (cant_enviar > 0) {
                status = conexion_enviar_mensaje(&conexion, enviar, sizeof(char) * cant_enviar);
                if (status != OK) {
                    free(enviar);
                    fprintf(stderr, "Error al enviar el mensaje\n");
                    return ERROR;
                }
            }
            free(enviar);
        }else if (!strcmp(mensaje_respuesta, "resultados")) {
            free(mensaje_respuesta);
            terminar = true;
            status = conexion_recibir_mensaje2(&conexion, &mensaje_respuesta);
            if (status <= 0) {
                fprintf(stderr, "Error al recibir dump de variables\n");
                return ERROR;
            }
            fprintf(stdout, "%s\n", (char*) mensaje_respuesta);
        }
    }

    free(mensaje_respuesta);
    conexion_destruir(&conexion);

    return OK;
}

int main(int argc, char *argv[]){
    FILE* archivo;

    if (argc > 1) {
        if (!strcmp(argv[1], "server")){
            if (argc != 3) {
                fprintf(stderr,"Cantidad de parametros invalida\n");
                return ERROR;
            }
            iniciar_servidor(argv[2]);
        }

        /**./tp client ​ <host> <port>​ ​ <N> ​ [ ​ <filename>​ ]*/
        if (!strcmp(argv[1], "client")){
            if ((argc < 5) || (argc>6)) {
                fprintf(stderr,"Cantidad de parametros invalida\n");
                return ERROR;
            } else if (argc == 5){
                iniciar_cliente(argv[2], argv[3], atoi(argv[4]), stdin);
            } else {
                archivo = fopen(argv[5], "r");
                if (!archivo) {
                    fprintf(stderr, "No se pudo abrir el archivo\n");
                    return ERROR;
                }
                iniciar_cliente(argv[2], argv[3], atoi(argv[4]), archivo);
                fclose(archivo);
            }
        }
    }

    return OK;
}
