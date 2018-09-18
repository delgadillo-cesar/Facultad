#ifndef CPU_H
#define CPU_H
#include <iostream>
#include <stdio.h>
#include <inttypes.h>

using namespace std;


class Cpu
{
    public:
        Cpu(FILE* archivo);
        void procesar();
        ~Cpu();

    protected:

    private:
        FILE* archivo;
        void procesar_direccion(uint32_t una_direccion);
};

#endif // CPU_H
