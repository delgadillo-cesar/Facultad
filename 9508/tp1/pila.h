#ifndef __PILA_H__
#define __PILA_H__
#include <stdlib.h>

typedef struct nodo_stack {
    void* dato;
    struct nodo_stack *anterior;
} nodo_pila;

typedef struct {
    nodo_pila *ultimo;
    size_t tamanio_elemento;
} pila_t;


int pila_crear(pila_t* la_pila, size_t tamanio_elemento);

int pila_agregar(pila_t* la_pila, void* elemento);

int pila_sacar(pila_t* la_pila, void* elemento);

void pila_destruir(pila_t* la_pila);

#endif
