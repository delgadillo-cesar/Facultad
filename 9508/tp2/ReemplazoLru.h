#ifndef REEMPLAZOLRU_H
#define REEMPLAZOLRU_H
#include "TipoReemplazo.h"

using namespace std;

class ReemplazoLru : public TipoReemplazo {
    public:
        ReemplazoLru();
        ~ReemplazoLru();
        virtual uint32_t agregar_direccion(uint32_t una_direccion) override;

    protected:

    private:
        int buscar(uint32_t una_direccion);
};

#endif // REEMPLAZOLRU_H
