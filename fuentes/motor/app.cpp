#include "app.h"
#include <iostream>

void Aplicacion::actualizar (double delta) {
    //Usamos static para que la variable no se destruya al salir de la funcion
    static double tiempoSimuladoTotal = 0.0;
    tiempoSimuladoTotal += delta;

    std::cout << "Frame logico. Delta fijo: " << delta
    << "s | Tiempo acumulado: " << tiempoSimuladoTotal << "s\n";
}