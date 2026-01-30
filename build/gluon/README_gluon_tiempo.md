Descripcion:
    Sistema de tiempo determinista que controla 60FPS o el que se establezca en <PASO_FIJO.>

    La prueba hace un bucle de un (1) segundo de duracion, donde se puede apreciar el funcionamiento del <acumulador> y <deltaMaximo>.

    La prueba se ejecuta en consola, en Debian 13 y Termux(en Android).

    La razon por la que uso Termux y no el NDK de Google, es simplemente porque:

   1.  Es mas facil para mi cuando estoy fuera de mi casa, me permite compilar en mi telefono Samsung Galaxy A13 4g, una antiguedad(en cosntruccion...)
------------------------------------------------------------------------------

Comando para compilar el nucleo.
Clang.
g++ -std=c++17 motor/main.cpp motor/codigo/nucleo/*.cpp -o build/chaos
