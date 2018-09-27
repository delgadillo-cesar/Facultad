#ifndef CACHEASOCIATIVA_H
#define CACHEASOCIATIVA_H
#include "Cache.h"
#include <set>
#include <map>
#include <string>
#include "TipoReemplazo.h"
#include "Logueador.h"

#define TIPO_FIFO 0
#define TIPO_LRU 1

class CacheAsociativa : public Cache {
    public:
        CacheAsociativa(std::map<std::string, std::string> config,
                        int tipo_reemp, Logueador& loger);
        ~CacheAsociativa();

    protected:
        virtual int buscar_en_memoria(uint32_t un_tag);
        virtual int agregar_en_memoria(uint32_t un_tag);

    private:
        TipoReemplazo *tipo;
        std::set<uint32_t> memoria;
        uint32_t cantidad_elementos;
};

#endif // CACHEASOCIATIVA_H
