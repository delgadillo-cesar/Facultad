#include <iostream>
#include <string>
#include <map>
#include "CacheDirecta.h"
#include "Logueador.h"

static const int MISS = 0;
static const int HIT = 1;
static const int OK = 0;
static const int ERROR = -1;

CacheDirecta::CacheDirecta(std::map<std::string, std::string> config,
                           Logueador& loger) : Cache(config, loger) {
    this->cant_bloques = tamanio / tamanio_linea;
}

int CacheDirecta::buscar_en_memoria(uint32_t un_tag) {
    int r = MISS;
    uint32_t bloque = un_tag % this->cant_bloques;

    std::map<uint32_t,uint32_t>::iterator it = this->memoria.find(bloque);
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
