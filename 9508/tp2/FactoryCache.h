#ifndef FACTORYCACHE_H
#define FACTORYCACHE_H
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "Cache.h"
#include "TipoReemplazo.h"
#include "Logueador.h"

class FactoryCache
{
    public:
        explicit FactoryCache(Logueador& loger);
        ~FactoryCache();
        Cache* crear_cache(std::filebuf especificaciones);
        Cache* crear_cache_directa(std::map<std::string, std::string> config);
        Cache* crear_cache_asociativa(
                          std::map<std::string, std::string> config, int tipo);

    protected:
    private:
        Logueador* loger;
};

#endif // FACTORYCACHE_H
