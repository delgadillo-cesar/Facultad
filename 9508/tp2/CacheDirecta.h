#ifndef CACHEDIRECTA_H
#define CACHEDIRECTA_H
#include "Cache.h"

using namespace std;

class CacheDirecta : public Cache {
    public:
        CacheDirecta(map<string, string> config);
        ~CacheDirecta();
        virtual int buscar_direccion(uint32_t una_direccion);

    protected:

    private:
        int cant_bloques;
        map<uint32_t, uint32_t> memoria;

        virtual int buscar_en_memoria(uint32_t un_tag);
        virtual int agregar_en_memoria(uint32_t un_tag);

};

#endif // CACHEDIRECTA_H
