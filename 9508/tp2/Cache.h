#ifndef CACHE_H
#define CACHE_H
#include <inttypes.h>
#include <map>

using namespace std;


class Cache
{
    public:
        Cache(int tamanio, int tamanio_liena, bool debug);
        Cache(map<string, string> config);
        virtual ~Cache();
        virtual int buscar_direccion(uint32_t una_direccion) = 0;

    protected:
        int tamanio;
        int tamanio_linea;
        bool debug;

        int buscar_tag(uint32_t un_tag);
        virtual int buscar_en_memoria(uint32_t una_tag) = 0;
        int agregar_tag(uint32_t un_tag);
        virtual int agregar_en_memoria(uint32_t una_tag) = 0;

    private:
        uint32_t hits;
        uint32_t misses;
};

#endif // CACHE_H
