#include "CacheAsociativa.h"
#include <iostream>

CacheAsociativa::CacheAsociativa(int tamanio, int tamanio_liena, TipoReemplazo *tipo) : Cache(tamanio, tamanio_liena) {
    this->tipo = tipo;
}

int CacheAsociativa::buscar_direccion(string una_direccion) {
    std::set<string>::iterator it;
    it = this->memoria.find(una_direccion);

    if (it != this->memoria.end()) {
    } else {
        this->agregar_direccion(una_direccion);
    }

    return 0;
}

void CacheAsociativa::agregar_direccion(string una_direccion) {
    string quitar;

    this->memoria.insert(una_direccion);
    quitar = this->tipo->agregar_direccion(una_direccion);

    if (!quitar.empty()) {
        this->memoria.erase(quitar);
    }
}

CacheAsociativa::~CacheAsociativa() {
    //dtor
}
