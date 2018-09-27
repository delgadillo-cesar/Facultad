#ifndef REEMPLAZOFIFO_H
#define REEMPLAZOFIFO_H
#include <deque>
#include "TipoReemplazo.h"

class ReemplazoFifo : public TipoReemplazo {
    public:
        ReemplazoFifo();
        ~ReemplazoFifo();
        virtual void agregar_tag(uint32_t un_tag);
        virtual void actualizar_tag(uint32_t un_tag);
        virtual uint32_t tag_para_quitar();

    protected:
    private:
        std::deque<uint32_t> cola;
};

#endif // REEMPLAZOFIFO_H
