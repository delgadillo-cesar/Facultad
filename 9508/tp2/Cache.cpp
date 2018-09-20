#include "Cache.h"
#include <iostream>
#include <iomanip>

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

int Cache::buscar_direccion(uint32_t una_direccion) {
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

void Cache::logear_direccion(uint32_t una_direccion, int status) {
    string tag_debug;
    if (status == HIT) tag_debug = "Hit: ";
    if (status == MISS) tag_debug = "Miss: ";

    if (this->debug) {
        cout << internal << setfill('0');
        cout << tag_debug << "0x"
             << hex << setw(8) << una_direccion
             << endl;
        cout << dec;
    }
}

void Cache::impimir_informe() {
    cout << endl << "# Informe" << endl << endl;
    cout << "* Total de hits: " << this->hits << endl;
    cout << "* Total de misses: " << this->misses;
}

Cache::~Cache() {
}
