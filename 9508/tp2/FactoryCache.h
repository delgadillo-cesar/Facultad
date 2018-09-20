#ifndef FACTORYCACHE_H
#define FACTORYCACHE_H
#include <iostream>
#include <fstream>
#include "Cache.h"
#include "TipoReemplazo.h"
#include <map>

using namespace std;

class FactoryCache
{
    public:
        FactoryCache();
        ~FactoryCache();
        Cache* crear_cache();
        Cache* crear_cache(filebuf especificaciones);
        Cache* crear_cache_directa (map<string, string> config);
        Cache* crear_cache_asociativa(map<string, string> configuracion,
                                      TipoReemplazo *tipo);

    protected:

    private:
};

#endif // FACTORYCACHE_H
