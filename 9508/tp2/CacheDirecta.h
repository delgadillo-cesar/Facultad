#ifndef CACHEDIRECTA_H
#define CACHEDIRECTA_H
#include <map>
#include <string>
#include "Cache.h"
#include "Logueador.h"

class CacheDirecta : public Cache {
    public:
        CacheDirecta(std::map<std::string, std::string> config,
                     Logueador& loger);
        ~CacheDirecta();

    protected:
        virtual int buscar_en_memoria(uint32_t un_tag);
        virtual int agregar_en_memoria(uint32_t un_tag);

    private:
        int cant_bloques;
        std::map<uint32_t, uint32_t> memoria;
};

#endif // CACHEDIRECTA_H
