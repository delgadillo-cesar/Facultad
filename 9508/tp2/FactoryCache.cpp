#include "FactoryCache.h"
#include "CacheDirecta.h"
#include "ReemplazoFifo.h"
#include "ReemplazoLru.h"

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

    return nullptr;
}

Cache* FactoryCache::crear_cache_directa (map<string, string> config) {
    Cache* cache = new CacheDirecta (config);
    return cache;
}

Cache* FactoryCache::crear_cache_asociativa(map<string, string> config,
                                            TipoReemplazo *tipo) {
//associative-fifo
    if (config["cache type"] == "associative-fifo") {
        ReemplazoFifo fifo;
        Cache* cache = new CacheAsociativa(config, move(fifo));
    }

    if (config["cache type"] == "associative-lru") {
    }

    return nullptr;
}


FactoryCache::~FactoryCache(){
}
