#ifndef CPU_H
#define CPU_H
#include <iostream>
#include <stdio.h>
#include <inttypes.h>
#include <fstream>
#include <string>
#include "Cache.h"


class Cpu
{
    public:
        Cpu(std::filebuf archivo, Cache *cache, Logueador& loger);
        void procesar();
        ~Cpu();
        int operator()();

    protected:
    private:
        std::filebuf archivo;
        Cache* cache;
        Logueador* loger;
        int procesar_direccion(uint32_t una_direccion);
};

#endif // CPU_H
