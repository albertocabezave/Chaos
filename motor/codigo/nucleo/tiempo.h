#ifndef TIEMPO_H
#define TIEMPO_H

#include <chrono>

class Tiempo {
    public:
        Tiempo();

        void actualizar();

        double obtenerDelta() const;

        void pausar();
        void reanudar();
        bool estaPausado() const;

        void establecerEscala(double escala);
        double obtenerEscala() const;

    private:
        using Reloj = std::chrono::steady_clock;
        
        Reloj::time_point tiempoAnterior;
        
        double deltaSimulacion; // Es double para poder hacer el paso fijo de 60 FPS

        bool pausado;

        const double deltaMaximo = 0.1; // segundos

        double escalaTiempo;

};

#endif
