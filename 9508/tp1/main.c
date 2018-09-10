#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servidor.h"
#include "cliente.h"

#define OK 0
#define ERROR 1



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
