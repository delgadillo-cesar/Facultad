#ifndef CACHE_H
#define CACHE_H

#include <string>

using namespace std;

class Cache
{
    public:
        Cache(int tamanio, int tamanio_liena);
        ~Cache();
        virtual int buscar_direccion(string una_direccion) = 0;
        virtual void agregar_direccion(string una_direccion) = 0;

    protected:
        int tamanio;
        int tamanio_linea;

    private:
};

#endif // CACHE_H
