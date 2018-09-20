#include <iostream>
#include <fstream>
#include "Cache.h"
#include "CacheDirecta.h"
#include "CacheAsociativa.h"
#include "TipoReemplazo.h"
#include "ReemplazoFifo.h"
#include "ReemplazoLru.h"
#include "Cpu.h"
#include <map>
#include "FactoryCache.h"

using namespace std;

static const int OK = 0;
static const int ERROR = 1;


int main(int argc, char* argv[]) {
    filebuf fb_e;
    if (!fb_e.open ("config.cfg",ios::in)) {
        return ERROR;
    }

    FactoryCache fc;
    Cache* cache = fc.crear_cache(move(fb_e));

    filebuf fb;
    if (!fb.open ("cpu-00.bin",ios::in)) {
        return ERROR;
    }

    Cpu cpu(move(fb), cache);
    cpu.procesar();
    cache->impimir_informe();
    delete cache;

    return 0;
}
