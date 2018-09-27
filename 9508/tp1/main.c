#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servidor.h"
#include "cliente.h"

static const int OK = 0;
static const int ERROR = 1;

static const int CLIENT_SERVER = 1;
static const int CLIENT_HOST = 2;
static const int SERVER_PORT = 2;
static const int CLIENT_PORT = 3;
static const int CLIENT_MEM = 4;
static const int CLIENT_PROG = 5;

static const int SERVER_PARAMS = 3;
static const int CLIENT_PARAM_MIN = 5;
static const int CLIENT_PARAM_MAX = 6;


int main(int argc, char *argv[]){
    FILE* arch;
    int s = OK;

    if (argc > 1) {
        if (!strcmp(argv[CLIENT_SERVER], "server")){
            if (argc != SERVER_PARAMS) {
                fprintf(stderr,"Cantidad de parametros invalida\n");
                return ERROR;
            }
            s = iniciar_servidor(argv[SERVER_PORT]);
        }

        if (!strcmp(argv[CLIENT_SERVER], "client")){
            if ((argc < CLIENT_PARAM_MIN) || (argc > CLIENT_PARAM_MAX)) {
                fprintf(stderr,"Cantidad de parametros invalida\n");
                return ERROR;
            } else if (argc == 5){
                s = iniciar_cliente(argv[CLIENT_HOST],
                                    argv[CLIENT_PORT],
                                    atoi(argv[CLIENT_MEM]),
                                    stdin);
            } else {
                arch = fopen(argv[CLIENT_PROG], "r");
                if (!arch) {
                    fprintf(stderr, "No se pudo abrir el archivo\n");
                    return ERROR;
                }
                s = iniciar_cliente(argv[CLIENT_HOST],
                                    argv[CLIENT_PORT],
                                    atoi(argv[CLIENT_MEM]),
                                    arch);
                fclose(arch);
            }
        }
    }

    return s;
}

