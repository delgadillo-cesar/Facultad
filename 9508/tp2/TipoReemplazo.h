#ifndef TIPOREEMPLAZO_H
#define TIPOREEMPLAZO_H

#include <deque>
#include <string>

using namespace std;

class TipoReemplazo
{
    public:
        TipoReemplazo();
        virtual ~TipoReemplazo();
        virtual void agregar_tag(uint32_t un_tag) = 0;
        virtual void actualizar_tag(uint32_t un_tag) = 0;
        virtual uint32_t tag_para_quitar() = 0;
//        void set_cantidad_elementos(uint32_t cantidad);

    protected:
        uint32_t cantidad;

    private:
};

#endif // TIPOREEMPLAZO_H
