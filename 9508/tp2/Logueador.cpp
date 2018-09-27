#include "Logueador.h"
#include <iostream>
#include <string>
#include <vector>


Logueador::Logueador() {
    this->mutex_err = new std::mutex();
    this->mutex_out = new std::mutex();
}

void Logueador::loguear_salida(std::string texto) {
    this->mutex_out->lock();
    std::cout << texto << std::endl;
    this->mutex_out->unlock();
}

void Logueador::loguear_salida(std::vector<std::string> lineas) {
    this->mutex_out->lock();

    for (const std::string& texto : lineas)
        std::cout << texto << std::endl;

    this->mutex_out->unlock();
}

void Logueador::loguear_error(std::string texto) {
    this->mutex_err->lock();
    std::cerr << texto << std::endl;
    this->mutex_err->unlock();
}

void Logueador::loguear_error(std::vector<std::string> lineas) {
    this->mutex_out->lock();

    for (const std::string& texto : lineas)
        std::cerr << texto << std::endl;

    this->mutex_out->unlock();
}

Logueador::~Logueador() {
    delete mutex_err;
    delete mutex_out;
}

Logueador& Logueador::operator=(const Logueador& otro) {
    if (this != &otro) {
    }
    return *this;
}
