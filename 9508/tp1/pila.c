#include <string.h>
#include "pila.h"

#define OK 0
#define ERROR 1
#define CANTIDAD_INCREMENTO 10


int pila_crear(pila_t* la_pila, size_t tamanio_elemento) {
    la_pila->tamanio_elemento = tamanio_elemento;
    la_pila->ultimo = NULL;

    return OK;
}

int pila_agregar(pila_t* la_pila, void* elemento) {
    nodo_pila* nodo = malloc(sizeof(nodo_pila));
    if (!nodo)
        return ERROR;

    nodo->dato = malloc(la_pila->tamanio_elemento);
    if (!nodo->dato)
        return ERROR;

    memcpy(nodo->dato, elemento, la_pila->tamanio_elemento);
    nodo->anterior = la_pila->ultimo;

    la_pila->ultimo = nodo;

    return OK;
}

int pila_sacar(pila_t* la_pila, void* elemento) {
    if (la_pila->ultimo == NULL)
        return ERROR;

    memcpy(elemento, la_pila->ultimo->dato, la_pila->tamanio_elemento);

    nodo_pila *aux = la_pila->ultimo;
    la_pila->ultimo = aux->anterior;
    free(aux->dato);
    free(aux);

    return OK;
}

void pila_destruir(pila_t* la_pila) {
    nodo_pila *aux;

    while (la_pila->ultimo) {
        aux = la_pila->ultimo;
        la_pila->ultimo = aux->anterior;
        free(aux->dato);
        free(aux);
    }
}
