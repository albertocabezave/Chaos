#include "motor.h"
#include <iostream>

Motor::Motor () {}

void Motor::ejecutar() {
    const double FPS_OBJETIVO = 60.0;
    const double PASO_FIJO = 1.0 / FPS_OBJETIVO;

    double acumulador = 0.0;
    int frames = 0;

    std::cout << "Motor iniciado.\n";

    while (frames < 60) {
        tiempo.actualizar();
        double deltaReal = tiempo.obtenerDelta();

        acumulador += deltaReal;

        while (acumulador >= PASO_FIJO) {
            aplicacion.actualizar(PASO_FIJO);
            acumulador -= PASO_FIJO;
            frames++;
        }
    }

    std::cout << "Motor detenido. \n";
}