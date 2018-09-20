#include "TipoReemplazo.h"

TipoReemplazo::TipoReemplazo() {
    this->cantidad = 0;
}

void TipoReemplazo::set_cantidad_elementos(uint32_t cantidad) {
    this->cantidad = cantidad;
}

TipoReemplazo::~TipoReemplazo() {
}
