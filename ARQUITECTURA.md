El programa principal (Motor) controla el bule de simulacion.
Un modulo de Tiempo mide intervalos reales entre interacciones 
y entrega un delta de simulacion (en segundos). El Motor acumula
ese delta y ejecuta la logica de la Aplicacion en pasos fijos (fixed step)
para conseguir resultados estables y reproducibles.

2. Componentes del sistema y correspondencia con archivos.

  - Punton de entrada:
    - motor/main.cpp - Crea Motor y arranca ejecutar()

  - Nucleo del motor (sheduler y bucle principal):
    - motor/codigo/nucleo/motor,h (declaraciones)
    - motor/codigo/nucleo/motor.cpp (implementacion del bucle)

  - Modulo Tiempo: mide intervalos, aplica clamp (deltaMaximo), escala
    y pausa:
    - motor/codigo/nucleo/tiempo.h (interfaz)
    - motor/codigo/nucleo/tiempo.cpp (implementacion)
