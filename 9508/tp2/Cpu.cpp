#include "Cpu.h"

using namespace std;


Cpu::Cpu(FILE* archivo) {
    this->archivo = archivo;

}

void Cpu::procesar() {
    uint8_t leido = fgetc(this->archivo);
    uint32_t buffer = 0;

    while (!feof(this->archivo)) {

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

        leido = fgetc(this->archivo);
        if (leido == '\n' || feof(this->archivo)) {
            this->procesar_direccion(buffer);
            buffer = 0;
            leido = fgetc(this->archivo);
        }
    }
}


void Cpu::procesar_direccion(uint32_t una_direccion) {
    printf("Direccion: %08X\n", una_direccion);
}


Cpu::~Cpu() {
}
