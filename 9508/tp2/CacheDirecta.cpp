#include <iostream>
#include "CacheDirecta.h"

CacheDirecta::CacheDirecta(int tamanio, int tamanio_liena) : Cache(tamanio, tamanio_liena) {
    //ctor
}

int CacheDirecta::buscar_direccion(string una_direccion) {
    cout << "Buscar Directo" << endl;
    return 0;
}

void CacheDirecta::agregar_direccion(string una_direccion) {
    cout << "Agregar Directo" << endl;
}


CacheDirecta::~CacheDirecta() {
    //dtor
}
