#ifndef TIPOREEMPLAZO_H
#define TIPOREEMPLAZO_H

#include <deque>
#include <string>

using namespace std;

class TipoReemplazo
{
    public:
        TipoReemplazo();
        ~TipoReemplazo();
        virtual uint32_t agregar_direccion(uint32_t una_direccion) = 0;
        void set_cantidad_elementos(uint32_t cantidad);

    protected:
        uint32_t cantidad;

    private:
};

#endif // TIPOREEMPLAZO_H
