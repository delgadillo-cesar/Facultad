#include "ReemplazoFifo.h"
#include <iostream>

ReemplazoFifo::ReemplazoFifo() : TipoReemplazo() {
}

void ReemplazoFifo::agregar_tag(uint32_t un_tag) {
    this->cola.push_back(un_tag);
}

void ReemplazoFifo::actualizar_tag(uint32_t un_tag) {
}

uint32_t ReemplazoFifo::tag_para_quitar() {
    uint32_t quitar = this->cola.front();
    this->cola.pop_front();

    return quitar;
}

ReemplazoFifo::~ReemplazoFifo() {
}
