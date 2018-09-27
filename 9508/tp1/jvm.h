#ifndef __JVM_H__
#define __JVM_H__
#include "pila.h"
#include "programa.h"

typedef struct  {
    pila_t pila;
    int *memoria;
    int tamanio_memoria;
} jvm_t;

int jvm_crear(jvm_t *la_jvm, int* memoria, size_t tamanio_memoria);

int jvm_procesar(jvm_t *la_jvm, programa_t *programa);

int jvm_finalizar(jvm_t *la_jvm);

#endif
