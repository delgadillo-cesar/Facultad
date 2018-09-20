#include "ReemplazoLru.h"
#include <iostream>
#include <algorithm>

ReemplazoLru::ReemplazoLru() : TipoReemplazo() {
}

void ReemplazoLru::agregar_tag(uint32_t un_tag) {
    this->cola.push_back(un_tag);
}

void ReemplazoLru::actualizar_tag(uint32_t un_tag) {
    deque<uint32_t>::iterator it = find(this->cola.begin(), this->cola.end(), un_tag);
    if (it != this->cola.end())
        this->cola.erase(it);

    this->cola.push_back(un_tag);
}

uint32_t ReemplazoLru::tag_para_quitar() {
    uint32_t quitar = this->cola.front();
    this->cola.pop_front();

    return quitar;
}

ReemplazoLru::~ReemplazoLru() {
}
