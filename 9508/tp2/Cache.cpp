#include "Cache.h"
#include <iostream>

static const int MISS = 0;
static const int HIT = 1;
static const int ERROR = -1;

Cache::Cache(int tamanio, int tamanio_liena, bool debug) {
    this->tamanio = tamanio;
    this->tamanio_linea = tamanio_liena;
    this->debug = debug;
    this->hits = 0;
    this->misses = 0;
}

Cache::Cache(map<string, string> config) {
    this->tamanio = stoi(config["cache size"]);
    this->tamanio_linea = stoi(config["line size"]);
    this->debug = config["debug"] == "true"?true:false;
    this->hits = 0;
    this->misses = 0;

    cout << "# Cache Inicializada" << endl;
    cout << endl;
    cout << "* Fabricante: " << config["vendor_id"] << endl;
    cout << "* Modelo: " << config["model name"] << endl;
    cout << "* Cpu MHz: " << config["cpu MHz"] << endl;
    cout << endl;
}


int Cache::buscar_tag(uint32_t un_tag) {
    int result =  this->buscar_en_memoria(un_tag);

    if (result == HIT) this->hits++;
    if (result == MISS) this->misses++;
    return result;
}

int Cache::agregar_tag(uint32_t un_tag) {
    int result = this->agregar_en_memoria(un_tag);
    return result;
}

Cache::~Cache() {
    cout << endl << "# Informe" << endl << endl;
    cout << "* Total de hits: " << this->hits << endl;
    cout << "* Total de misses: " << this->misses << endl;
}
