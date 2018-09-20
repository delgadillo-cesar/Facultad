#ifndef CACHEASOCIATIVA_H
#define CACHEASOCIATIVA_H
#include "Cache.h"
#include "TipoReemplazo.h"
#include <set>

#define TIPO_FIFO 0
#define TIPO_LRU 1

using namespace std;

class CacheAsociativa : public Cache {
    public:
        CacheAsociativa(map<string, string> config, int tipo_reemp);
        ~CacheAsociativa();

    protected:

    private:
        TipoReemplazo *tipo;
        set<uint32_t> memoria;
        uint32_t cantidad_elementos;

        virtual int buscar_en_memoria(uint32_t un_tag);
        virtual int agregar_en_memoria(uint32_t un_tag);

};

#endif // CACHEASOCIATIVA_H
