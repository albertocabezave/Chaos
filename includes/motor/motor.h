#ifndef MOTOR_H
#define MOTOR_H

#include "tiempo.h"
#include "app.h"

class Motor {
    public:
        Motor();
        void ejecutar();

    private:
        Tiempo tiempo;
        Aplicacion aplicacion;
};

#endif