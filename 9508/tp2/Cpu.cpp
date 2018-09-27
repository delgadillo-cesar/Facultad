#include "Cpu.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

static const int ERROR = 1;
static const int ERROR_CACHE = -1;
static const int OK = 0;

Cpu::Cpu(std::filebuf archivo, Cache *cache, Logueador& loger) {
    this->archivo = move(archivo);
    this->cache = cache;
    this->loger = &loger;
}

void Cpu::procesar() {
    std::istream arch(&this->archivo);
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
                std::stringstream s_direccion;
                s_direccion << std::setfill('0')
                            << std::setw(8)
                            << std::hex
                            << buffer;

                std::vector<std::string> lineas;
                std::string texto = "Direccion invalida: 0x";
                texto += s_direccion.str();
                lineas.push_back(texto);
                lineas.push_back("Abortando");
                this->loger->loguear_error(lineas);
                break;
            }
            buffer = 0;
            leido = arch.get();
        }
    }
}


int Cpu::procesar_direccion(uint32_t una_direccion) {
    if (una_direccion % 4 != 0) {
        return ERROR;
    }

    int result = this->cache->buscar_direccion(una_direccion);
    if (result == ERROR_CACHE) {
        return ERROR;
    }

    return OK;
}

int Cpu::operator()() {
    this->procesar();
    return 0;
}

Cpu::~Cpu() {
    this->archivo.close();
}
