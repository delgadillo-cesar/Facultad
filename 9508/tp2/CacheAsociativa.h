#ifndef CACHEASOCIATIVA_H
#define CACHEASOCIATIVA_H
#include "Cache.h"
#include "TipoReemplazo.h"
#include <set>

using namespace std;

class CacheAsociativa : public Cache {
    public:
        CacheAsociativa(int tamanio, int tamanio_liena, TipoReemplazo *tipo);
        ~CacheAsociativa();
        virtual int buscar_direccion(string una_direccion);
        virtual void agregar_direccion(string una_direccion);

    protected:

    private:
        TipoReemplazo *tipo;
        set<string> memoria;
};

#endif // CACHEASOCIATIVA_H
