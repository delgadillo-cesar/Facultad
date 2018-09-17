#include "ReemplazoFifo.h"
#include <iostream>

ReemplazoFifo::ReemplazoFifo(int cant_elementos) : TipoReemplazo() {
    this->cantidad = cant_elementos;
}

string ReemplazoFifo::agregar_direccion(string una_direccion) {
    string quitar = "";

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
