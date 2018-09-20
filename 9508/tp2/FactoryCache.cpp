#include "FactoryCache.h"
#include "CacheDirecta.h"
#include "CacheAsociativa.h"

FactoryCache::FactoryCache(){
}

Cache* FactoryCache::crear_cache(filebuf especificaciones) {
    map<string, string> configuracion;
    istream arch(&especificaciones);
    char leido = arch.get();
    string buffer = "";

    while (arch) {
        if (leido == '\n' || !arch) {
            int pos_separador = buffer.find("=");
            string clave = buffer.substr(0, pos_separador);
            string valor = buffer.substr(pos_separador + 1,
                                         buffer.length() - pos_separador);
            configuracion[clave] = valor;
            buffer = "";
        } else {
            buffer += leido;
        }
        leido = arch.get();
    }

    especificaciones.close();
    if (configuracion["cache type"] == "direct") {
        return this->crear_cache_directa(configuracion);
    }

    if (configuracion["cache type"] == "associative-fifo") {
        return this->crear_cache_asociativa(configuracion, TIPO_FIFO);
    }

    if (configuracion["cache type"] == "associative-lru") {
        return this->crear_cache_asociativa(configuracion, TIPO_LRU);
    }
    return nullptr;
}

Cache* FactoryCache::crear_cache_directa (map<string, string> config) {
    Cache* cache = new CacheDirecta (config);
    return cache;
}

Cache* FactoryCache::crear_cache_asociativa(map<string, string> config,
                                            int tipo) {
    if (config["cache type"] == "associative-fifo") {
        Cache* cache = new CacheAsociativa(config, TIPO_FIFO);
        return cache;
    }

    if (config["cache type"] == "associative-lru") {
        Cache* cache = new CacheAsociativa(config, TIPO_LRU);
        return cache;
    }

    return nullptr;
}

FactoryCache::~FactoryCache(){
}
