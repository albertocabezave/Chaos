#include "tiempo.h"

Tiempo::Tiempo()
    :tiempoAnterior(Reloj::now()),
    deltaSimulacion(0.0),
    pausado(false),
    escalaTiempo(1.0) // Iniciamos a velocidad normal
{
}

void Tiempo::actualizar() {
    Reloj::time_point ahora = Reloj::now();

    // Traducimos tics en nanosegundos (100,200) a segundos (0.1,0.2) 
    std::chrono::duration<double> deltaReal = ahora - tiempoAnterior;
    tiempoAnterior = ahora;

    // Pausamos poniendo los valores en 0, para no detener el procesador
    if (pausado) {
        deltaSimulacion = 0.0;
        return;
    }

    double segundos = deltaReal.count();

    // Limitamos el tiempo real antes de aplicar escala
    if (segundos > deltaMaximo) {
        segundos = deltaMaximo;
    }
    
    // Aplicamos la escala de tiempo(Camara lenta/rapido)
    deltaSimulacion = segundos * escalaTiempo;
}

void Tiempo::establecerEscala(double escala) {
    if (escala >= 0.0) { // Evitamos escalas negativas
        escalaTiempo = escala;

    }
}

double Tiempo::obtenerEscala() const {
    return escalaTiempo;
}

double Tiempo::obtenerDelta() const {
    return deltaSimulacion;
}

void Tiempo::pausar() {
    pausado = true;
}

void Tiempo::reanudar() {
    pausado = false;
    tiempoAnterior = Reloj::now();
}

bool Tiempo::estaPausado() const {
    return pausado;
}