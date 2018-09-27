#include <map>
#include <string>
#include "FactoryCache.h"
#include "CacheDirecta.h"
#include "CacheAsociativa.h"
#include "Logueador.h"

FactoryCache::FactoryCache(Logueador& loger) : loger() {
    this->loger = &loger;
}

bool validaciones(std::map<std::string, std::string> configuracion) {
    bool validacion = true;

    if (configuracion.find("cache type") == configuracion.end()) {
        validacion = false;
        std::cerr << "Falta parametro 'cache type'" << std::endl;
    }

    if (configuracion.find("cache size") == configuracion.end()) {
        validacion = false;
        std::cerr << "Falta parametro 'cache size'" << std::endl;
    }

    if (configuracion.find("line size") == configuracion.end()) {
        validacion = false;
        std::cerr << "Falta parametro 'line size'" << std::endl;
    }

    return validacion;
}

Cache* FactoryCache::crear_cache(std::filebuf especificaciones) {
    std::map<std::string, std::string> configuracion;
    std::istream arch(&especificaciones);
    char leido = arch.get();
    std::string buffer = "";

    while (arch) {
        if (leido == '\n' || !arch) {
            int pos_separador = buffer.find("=");
            std::string clave = buffer.substr(0, pos_separador);
            std::string valor = buffer.substr(pos_separador + 1,
                                         buffer.length() - pos_separador);
            configuracion[clave] = valor;
            buffer = "";
        } else {
            buffer += leido;
        }
        leido = arch.get();
    }

    if (!validaciones(configuracion)) {
        return nullptr;
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

Cache* FactoryCache::crear_cache_directa(
                                 std::map<std::string, std::string> config) {
    Cache* cache = new CacheDirecta(config, *this->loger);
    return cache;
}

Cache* FactoryCache::crear_cache_asociativa(
                         std::map<std::string, std::string> config, int tipo) {
    if (config["cache type"] == "associative-fifo") {
        Cache* cache = new CacheAsociativa(config, TIPO_FIFO, *this->loger);
        return cache;
    }

    if (config["cache type"] == "associative-lru") {
        Cache* cache = new CacheAsociativa(config, TIPO_LRU, *this->loger);
        return cache;
    }

    return nullptr;
}

FactoryCache::~FactoryCache(){
}
