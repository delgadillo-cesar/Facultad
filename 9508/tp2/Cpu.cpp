#include "Cpu.h"
#include <iomanip>

using namespace std;

static const int ERROR = 1;
static const int ERROR_CACHE = -1;
static const int OK = 0;

Cpu::Cpu(filebuf archivo, Cache *cache) {
    this->archivo = move(archivo);
    this->cache = cache;
}

void Cpu::procesar() {
    istream arch(&this->archivo);
    uint8_t leido = arch.get();
    uint32_t buffer = 0;

    while (arch) {

        if ((leido >= 48) && (leido <= 57)) {
            leido -= 48;
            buffer = buffer << 4;
            buffer += leido;
        }

        if ((leido >= 65) && (leido <= 70)) {
            leido -= 55;
            buffer = buffer << 4;
            buffer += leido;
        }

        leido = arch.get();
        if (leido == '\n' || !arch) {
            if (this->procesar_direccion(buffer) == ERROR) {
                cerr << "Abortando" << endl;
                break;
            }
            buffer = 0;
            leido = arch.get();
        }
    }
}


int Cpu::procesar_direccion(uint32_t una_direccion) {
    if (una_direccion % 4 != 0) {
        cerr << internal
             << setfill('0');
        cerr << "Direccion invalida: 0x"
             << uppercase
             << hex << setw(8) << una_direccion
             << endl;
        return ERROR;
    }

    int result = this->cache->buscar_direccion(una_direccion);
    if ( result == ERROR_CACHE) {
        return ERROR;
    }

    return OK;
}


Cpu::~Cpu() {
    this->archivo.close();
}
