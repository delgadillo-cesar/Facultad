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
        virtual string agregar_direccion(string una_direccion) = 0;

    protected:

    private:
};

#endif // TIPOREEMPLAZO_H
