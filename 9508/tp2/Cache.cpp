#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "Cache.h"

static const int MISS = 0;
static const int HIT = 1;
static const int ERROR = -1;

Cache::Cache(std::map<std::string, std::string> config, Logueador& loger) {
    this->tamanio = stoi(config["cache size"]);
    this->tamanio_linea = stoi(config["line size"]);
    this->debug = config["debug"] == "true"?true:false;
    this->hits = 0;
    this->misses = 0;
    this->loger = &loger;

    std::vector<std::string> lineas;
    lineas.push_back("# Cache Inicializada");
    lineas.push_back("");
    lineas.push_back("* Fabricante: " + config["vendor_id"]);
    lineas.push_back("* Modelo: " + config["model name"]);
    lineas.push_back("* Cpu MHz: " + config["cpu MHz"]);
    lineas.push_back("");

    this->loger->loguear_salida(lineas);
}

int Cache::buscar_direccion(uint32_t una_direccion) {
    uint32_t len_offset = log2(this->tamanio_linea);
    uint32_t tag = una_direccion >> len_offset;

    uint32_t offset = una_direccion;
    offset = offset << (32 - len_offset);
    offset = offset >> (32 - len_offset);

    int result = this->buscar_tag(tag);

    if (result == ERROR) return ERROR;

    this->logear_direccion(una_direccion, result);
    return result;
}

int Cache::buscar_tag(uint32_t un_tag) {
    this->m.lock();
    int result =  this->buscar_en_memoria(un_tag);

    if (result == HIT)
        this->hits++;

    if (result == MISS) {
        this->misses++;
        this->agregar_tag(un_tag);
    }
    this->m.unlock();

    return result;
}

int Cache::agregar_tag(uint32_t un_tag) {
    int result = this->agregar_en_memoria(un_tag);
    return result;
}

void Cache::logear_direccion(uint32_t una_direccion, int status) {
    if (this->debug) {
        std::string tag_debug;
        if (status == HIT) tag_debug = "Hit: 0x";
        if (status == MISS) tag_debug = "Miss: 0x";

        std::stringstream s_direccion;
        s_direccion << std::setfill('0')
                    << std::setw(8)
                    << std::hex
                    << una_direccion;
        tag_debug += s_direccion.str();
        this->loger->loguear_salida(tag_debug);
    }
}

void Cache::impimir_informe() {
    std::vector<std::string> lineas;
    lineas.push_back("");
    lineas.push_back("# Informe");
    lineas.push_back("");
    lineas.push_back("* Total de hits: " + std::to_string(this->hits));
    lineas.push_back("* Total de misses: " + std::to_string(this->misses));

    this->loger->loguear_salida(lineas);
}

Cache::~Cache() {
}
