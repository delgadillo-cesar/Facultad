#ifndef CACHE_H
#define CACHE_H
#include <inttypes.h>
#include <map>
#include <cmath>
#include <iomanip>
#include <mutex>


using namespace std;


class Cache
{
    public:
        Cache(map<string, string> config);
        virtual ~Cache();
        int buscar_direccion(uint32_t una_direccion);
        void impimir_informe();

    protected:
        int tamanio;
        int tamanio_linea;
        bool debug;

        int buscar_tag(uint32_t un_tag);
        virtual int buscar_en_memoria(uint32_t una_tag) = 0;
        int agregar_tag(uint32_t un_tag);
        virtual int agregar_en_memoria(uint32_t una_tag) = 0;
        void logear_direccion(uint32_t una_direccion, int status);

    private:
        uint32_t hits;
        uint32_t misses;
        mutex m;
};

#endif // CACHE_H
