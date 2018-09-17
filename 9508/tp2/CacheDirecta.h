#ifndef CACHEDIRECTA_H
#define CACHEDIRECTA_H
#include "Cache.h"

using namespace std;

class CacheDirecta : public Cache {
    public:
        CacheDirecta(int tamanio, int tamanio_liena);
        ~CacheDirecta();
        virtual int buscar_direccion(string una_direccion);
        virtual void agregar_direccion(string una_direccion);

    protected:

    private:
};

#endif // CACHEDIRECTA_H
