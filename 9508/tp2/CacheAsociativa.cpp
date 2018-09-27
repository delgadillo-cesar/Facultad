#include "CacheAsociativa.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "ReemplazoFifo.h"
#include "ReemplazoLru.h"


static const int MISS = 0;
static const int HIT = 1;
static const int OK = 0;
static const int ERROR = -1;

CacheAsociativa::CacheAsociativa(std::map<std::string, std::string> config,
                                 int tipo_reemp, Logueador& loger)
               : Cache(config, loger) {
    if (tipo_reemp == TIPO_FIFO)
        this->tipo = new ReemplazoFifo();

    if (tipo_reemp == TIPO_LRU)
        this->tipo = new ReemplazoLru();

    this->cantidad_elementos = tamanio / tamanio_linea;
}

int CacheAsociativa::buscar_en_memoria(uint32_t un_tag) {
    std::set<uint32_t>::iterator it;
    it = this->memoria.find(un_tag);

    if (it != this->memoria.end()) {
        this->tipo->actualizar_tag(un_tag);
        return HIT;
    } else {
        return MISS;
    }
}

int CacheAsociativa::agregar_en_memoria(uint32_t un_tag) {
    this->memoria.insert(un_tag);

    this->tipo->agregar_tag(un_tag);

    if (this->memoria.size() > this->cantidad_elementos) {
        uint32_t quitar = this->tipo->tag_para_quitar();
        this->memoria.erase(quitar);
    }

    return OK;
}

CacheAsociativa::~CacheAsociativa() {
    delete this->tipo;//dtor
}
