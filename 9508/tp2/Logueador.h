#ifndef LOGUEADOR_H
#define LOGUEADOR_H
#include <string>
#include <mutex>
#include <vector>

class Logueador {
    public:
        Logueador();
        ~Logueador();
        Logueador& operator=(const Logueador& otro);
        void loguear_salida(std::string texto);
        void loguear_salida(std::vector<std::string> lineas);
        void loguear_error(std::string texto);
        void loguear_error(std::vector<std::string> lineas);

    protected:
    private:
        std::mutex* mutex_out;
        std::mutex* mutex_err;
};

#endif // LOGUEADOR_H
