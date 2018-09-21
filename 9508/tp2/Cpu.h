#ifndef CPU_H
#define CPU_H
#include <iostream>
#include <stdio.h>
#include <inttypes.h>
#include "Cache.h"
#include <fstream>


using namespace std;


class Cpu
{
    public:
        Cpu(filebuf archivo, Cache *cache);
        void procesar();
        ~Cpu();
        int operator()();

    protected:

    private:
        filebuf archivo;
        Cache* cache;
        int procesar_direccion(uint32_t una_direccion);
};

#endif // CPU_H
