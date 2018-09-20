#include <iostream>
#include <cmath>
#include <iomanip>
#include "CacheDirecta.h"

static const int MISS = 0;
static const int HIT = 1;
static const int ERROR = -1;

CacheDirecta::CacheDirecta(map<string, string> config) : Cache(config) {
    this->cant_bloques = tamanio / tamanio_linea;
}

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

    string tag_debug;
    if (result == MISS) {
        tag_debug = "Miss: ";
        result = MISS;
        this->agregar_tag(tag);
    } else {
        tag_debug = "Hit: ";
        result = HIT;
    }

    if (this->debug) {
        cout << internal << setfill('0');
        cout << tag_debug << "0x"
             << uppercase
             << hex << setw(8) << una_direccion
             << endl;
    }
    return result;
}

int CacheDirecta::buscar_en_memoria(uint32_t un_tag) {
    int r = 0;
    uint32_t bloque = un_tag % this->cant_bloques;

    if (this->memoria[bloque] == un_tag) {
        r = 1;
    } else {
    }
    return r;
}

int CacheDirecta::agregar_en_memoria(uint32_t un_tag) {
    int r = 0;
    uint32_t bloque = un_tag % this->cant_bloques;

    this->memoria[bloque] = un_tag;

    return r;
}

CacheDirecta::~CacheDirecta() {
}
