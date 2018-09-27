#ifndef REEMPLAZOLRU_H
#define REEMPLAZOLRU_H
#include <deque>
#include "TipoReemplazo.h"

class ReemplazoLru : public TipoReemplazo {
    public:
        ReemplazoLru();
        ~ReemplazoLru();
        virtual void agregar_tag(uint32_t un_tag);
        virtual void actualizar_tag(uint32_t un_tag);
        virtual uint32_t tag_para_quitar();

    protected:
    private:
        std::deque<uint32_t> cola;
};

#endif // REEMPLAZOLRU_H
