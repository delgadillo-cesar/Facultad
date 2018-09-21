#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "FactoryCache.h"
#include "Cpu.h"

using namespace std;

static const int OK = 0;
static const int ERROR = 1;
static const int ARCH_CFG = 1;
static const int ARCH_CPU0 = 2;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Cantidad de parametros insuficientes" << endl;
        cerr << "Se espera: <archivo.cfg> <cpu-01.bin> [<cpu-NN.bin>]"<< endl;
        return ERROR;
    }

    filebuf fb;
    if (!fb.open (argv[ARCH_CFG],ios::in)) {
        cerr << "No se pudo abrir el archivo de configuracion" << endl;
        return ERROR;
    }

    FactoryCache fc;
    Cache* cache = fc.crear_cache(move(fb));

    if (!cache) {
        cerr << "No se pudo crear la cache" << endl;
        return ERROR;
    }

    vector<Cpu*> cpus;
    for (int i = ARCH_CPU0; i < argc; i++) {
        filebuf fb_c;
        if (!fb_c.open (argv[i],ios::in)) {
            cerr << "No se pudo abrir el archivo " << argv[i] << endl;
            return ERROR;
        }
        Cpu* cpu = new Cpu(move(fb_c), cache);
        cpus.push_back(cpu);
    }


    vector<thread*> hilos;
    for (uint32_t i = 0; i < cpus.size(); i++) {
		hilos.push_back(new thread(ref(*(cpus[i]))));
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
