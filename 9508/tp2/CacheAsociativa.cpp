#include "CacheAsociativa.h"
#include <iostream>

CacheAsociativa::CacheAsociativa(int tamanio, int tamanio_liena, bool debug, TipoReemplazo *tipo)
               : Cache(tamanio, tamanio_liena, debug) {
    this->tipo = tipo;
    this->tipo->set_cantidad_elementos(tamanio / tamanio_liena);
}

int CacheAsociativa::buscar_direccion(uint32_t una_direccion) {
    std::set<uint32_t>::iterator it;
    it = this->memoria.find(una_direccion);

    if (it != this->memoria.end()) {
    } else {
        this->agregar_direccion(una_direccion);
    }

    return 0;
}

void CacheAsociativa::agregar_direccion(uint32_t una_direccion) {
    uint32_t quitar;

    this->memoria.insert(una_direccion);
    quitar = this->tipo->agregar_direccion(una_direccion);

    if (!quitar) {
        this->memoria.erase(quitar);
    }
}

int CacheAsociativa::buscar_en_memoria(uint32_t un_tag) {
    return 0;
}

int CacheAsociativa::agregar_en_memoria(uint32_t un_tag) {
    return 0;
}

CacheAsociativa::~CacheAsociativa() {
    //dtor
}
