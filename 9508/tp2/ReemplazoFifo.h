#ifndef REEMPLAZOFIFO_H
#define REEMPLAZOFIFO_H
#include "TipoReemplazo.h"

using namespace std;

class ReemplazoFifo : public TipoReemplazo {
    public:
        ReemplazoFifo(int cant_elementos);
        ~ReemplazoFifo();
        virtual string agregar_direccion(string una_direccion) override;

    protected:

    private:
        int cantidad;
        deque<string> cola;
};

#endif // REEMPLAZOFIFO_H
