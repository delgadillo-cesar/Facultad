#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "FactoryCache.h"
#include "Logueador.h"
#include "Cpu.h"

static const int OK = 0;
static const int ERROR = 1;
static const int ARCH_CFG = 1;
static const int ARCH_CPU0 = 2;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Cantidad de parametros insuficientes" << std::endl;
        std::cerr << "Se espera: <archivo.cfg> <cpu-01.bin> [<cpu-NN.bin>]"
                  << std::endl;
        return ERROR;
    }

    std::filebuf fb;
    if (!fb.open(argv[ARCH_CFG],std::ios::in)) {
        std::cerr << "No se pudo abrir el archivo de configuracion"
                  << std::endl;
        return ERROR;
    }

    Logueador loger;
    FactoryCache fc(loger);
    Cache* cache = fc.crear_cache(std::move(fb));

    if (!cache) {
        std::cerr << "No se pudo crear la cache" << std::endl;
        return ERROR;
    }

    std::vector<Cpu*> cpus;
    for (int i = ARCH_CPU0; i < argc; i++) {
        std::filebuf fb_c;
        if (!fb_c.open(argv[i], std::ios::in)) {
            std::cerr << "No se pudo abrir el archivo " << argv[i]
                      << std::endl;
            return ERROR;
        }
        Cpu* cpu = new Cpu(std::move(fb_c), cache, loger);
        cpus.push_back(cpu);
    }


    std::vector<std::thread*> hilos;
    for (uint32_t i = 0; i < cpus.size(); i++) {
		hilos.push_back(new std::thread(std::ref(*(cpus[i]))));
    }

    for (uint32_t i = 0; i < cpus.size(); i++) {
		hilos[i]->join();
		delete hilos[i];
        delete cpus[i];
    }

    cache->impimir_informe();
    delete cache;

    return 0;
}
