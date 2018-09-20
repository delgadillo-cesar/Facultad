#include <iostream>
#include "CacheDirecta.h"

static const int MISS = 0;
static const int HIT = 1;
static const int OK = 0;
static const int ERROR = -1;

CacheDirecta::CacheDirecta(map<string, string> config) : Cache(config) {
    this->cant_bloques = tamanio / tamanio_linea;
}

/*
int CacheDirecta::buscar_direccion(uint32_t una_direccion) {
    uint32_t len_offset = log2(this->tamanio_linea);
    uint32_t tag = una_direccion >> len_offset;

    uint32_t offset = una_direccion;
    offset = offset << (32 - len_offset);
    offset = offset >> (32 - len_offset);


    int result = this->buscar_tag(tag);
    if (result == ERROR) {
        return ERROR;
    }

    if (result == MISS)
        this->agregar_tag(tag);

    this->logear_direccion(una_direccion, result);
    return result;
}
*/

int CacheDirecta::buscar_en_memoria(uint32_t un_tag) {
    int r = MISS;
    uint32_t bloque = un_tag % this->cant_bloques;

    map<uint32_t,uint32_t>::iterator it = this->memoria.find(bloque);
    if (it != this->memoria.end())
        if (this->memoria[bloque] == un_tag)
            r = HIT;

    return r;
}

int CacheDirecta::agregar_en_memoria(uint32_t un_tag) {
    uint32_t bloque = un_tag % this->cant_bloques;

    this->memoria[bloque] = un_tag;

    return OK;
}

CacheDirecta::~CacheDirecta() {
}
