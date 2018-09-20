#ifndef REEMPLAZOFIFO_H
#define REEMPLAZOFIFO_H
#include "TipoReemplazo.h"

using namespace std;

class ReemplazoFifo : public TipoReemplazo {
    public:
        ReemplazoFifo();
        ~ReemplazoFifo();
        virtual uint32_t agregar_direccion(uint32_t una_direccion) override;

    protected:

    private:
        deque<uint32_t> cola;
};

#endif // REEMPLAZOFIFO_H
