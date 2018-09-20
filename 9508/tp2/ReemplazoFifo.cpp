#include "ReemplazoFifo.h"
#include <iostream>

ReemplazoFifo::ReemplazoFifo() : TipoReemplazo() {
//    this->cantidad = cant_elementos;
}

uint32_t ReemplazoFifo::agregar_direccion(uint32_t una_direccion) {
    uint32_t quitar = 0;

    this->cola.push_back(una_direccion);

    if ((int)this->cola.size() > this->cantidad) {
        quitar = this->cola.front();
        this->cola.pop_front();
    }

    return quitar;
}

ReemplazoFifo::~ReemplazoFifo() {
    //dtor
}
