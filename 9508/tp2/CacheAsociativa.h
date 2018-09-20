#ifndef CACHEASOCIATIVA_H
#define CACHEASOCIATIVA_H
#include "Cache.h"
#include "TipoReemplazo.h"
#include <set>

using namespace std;

class CacheAsociativa : public Cache {
    public:
        CacheAsociativa(int tamanio, int tamanio_liena, bool debug, TipoReemplazo *tipo);
        ~CacheAsociativa();
        virtual int buscar_direccion(uint32_t una_direccion);
        virtual void agregar_direccion(uint32_t una_direccion);

    protected:

    private:
        TipoReemplazo *tipo;
        set<uint32_t> memoria;

        virtual int buscar_en_memoria(uint32_t un_tag);
        virtual int agregar_en_memoria(uint32_t un_tag);

};

#endif // CACHEASOCIATIVA_H
