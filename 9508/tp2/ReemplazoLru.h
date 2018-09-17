#ifndef REEMPLAZOLRU_H
#define REEMPLAZOLRU_H
#include "TipoReemplazo.h"

using namespace std;

class ReemplazoLru : public TipoReemplazo {
    public:
        ReemplazoLru(int algo);
        ~ReemplazoLru();
        virtual string agregar_direccion(string una_direccion) override;

    protected:

    private:
        int buscar(string una_direccion);
};

#endif // REEMPLAZOLRU_H
