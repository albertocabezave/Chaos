# Modulo Tiempo

# Documento tecnico.

# 1. Proposito del modulo.

    El modulo Tiempo es responsable de proporcionar al motor un control preciso, estable y seguro del tiempo de simulacion.
    Su objetivo principal es desacoplar el comportamiento del juego del reloj del sistema operativo, garantizando resultados consistentes independientes del rendimiento de la maquina o de eventos externos.

    Este modulo no mide fechas, no depede del reloj del sistema, y no controla el renderizado ni los FPS.
    Su unica responsabilidad es generar un Delta Time confiable para la logica del motor.

# 2. Problema que resuelve.

    Los sistemas operativos modernos utilizan relojes de alta precision que pueden:
        - Avanzar demasiado rapido si el programa no limita su ejecucion.
        - Verse afectado por cambios manuales de hora o sincronizacion de red.
        -Producir deltas de tiempo extremadamente grandes tras pausas, bloqueos o congelamientos.

Si estos valores se utilizan directamente en una simulacion, pueden provocar:
    - Saltos bruscos de posicion.
    - Explosiones en sistemas fisicos.
    - Resultados numericos inestales.
    - Comportamientos irreproducibles.

El modulo Tiempo existe para proteger la simulacion de estas condiciones.

# 3. Principios de diseño.

    El diseño del modulo Tiempo se basa en los siguientes principios:

 # 3.1 Reloj monotono.
    Se usa std::chrono::steady_clock, un reloj que:
        - Nunca retrocede.
        - No depende de la fecha del sistema.
        - Es ideal para medir duraciones.
    Esto garantiza que el tiempo medido siempre avance de forma consistente.

 # 3.2 Separacion entre tiempo real y tiempo de simulacion.
    El modulo distingue claramente entre:
        - Tiempo real: lo que mide el reloj del sistema.
        - Tiempo de simulacion: el tiempo que el motor decide aceptar.
    El motor no esta obligado a obedecer ciegamente al tiempo real.

 # 3.3 Limitacion del delta maximo.
    Para evitar saltos extremos, el sistema impone un limite maximo al delta de simulacion.
    Ejemplo:
        - Si el sistema reporta 2 segundos transcurridos.
        - El motor solo acepta, por ejemplo, 0.1 segundos.
    Esto asegura estabilidad numerica y visual.

 # 3.4 Pausa sin detener el hilo principal.
    El sistema permite pausar la simulacion devolviendo un delta de tiempo igual a cero, sin detener la ejecucion del programa ni bloquear el procesador.
    Esto permite:
        - Mantener interfaces activas.
        - Continuar renderizando.
        - Evitar bloqueos del sistema.

# 4. Responsabilidades del modulo.
    El modulo Tiempo es responsable de:
        - Medir el tiempo transcurrido entre frames.
        - Calcular el delta de simulacion.
        - Limitar el delta maximo permitido.
        - Permitir pausar y reanudar la simulacion.
        - Proporcionar el delta de forma clara al motor.

# 5. Responsabilidades que NO le pertenecen.
    El modulo tiempo no debe:
        - Controlar FPS.
        - Dormir o bloquear hilos.
        - Renderizar graficos.
        - Procesar input.
        - Conocer detalles del sistema operativo.
        - Manjear la logica del juego.
    Estas responsabilidades pertenecen a otros modulos del motor.

# 6. Interfaz publica.
    El modulo tiempo muestra las siguientes funciones principales que usara para operar:
        - actualizar()
            Calcula el tiempo que ha pasado desde la ultima actualizacion.
        - obtenerDelta()
            Devuelve el delta de simulacion en segundos.
        - pausar()
            Detiene la simulacion sin detener la ejecucion del programa.
        - reanudar()
            Reactiva la simulacion evitando saltos de tiempo.
        - estaPausado()
            Indica el estado actual del sistema de tiempo.
        - escalaTiempo()
            Permite simular camara lenta o camara rapida de forma segura.
# 7. Flujo de ejecucion tipico.
    En cada interaccion del bucle principal del motor:
        1. El motor llama a Tiempo::actualizar().
        2. El modulo calcula el tiempo real transcurrido.
        3. Se aplica la logica de pausa si corresponde.
        4. Se aplica la logica de escala de tiempo si corresponde.
        5. Se limita el delta maximo permitido.
        6. El delta de simulacion se almacena internamente.
        7. El motor obtiene el delta mediante obtenerDelta() y lo usa para la logica del juego.

# 8. Justificacion arquitectonica.
    Este diseño permite que :
        - La simulacion sea reproducible.
        - El motor sea portable a multiples plataformas.
        - Los calculos sean estables incluso en condiciones adversas.
        - El sistema puede usarse tanto en videojuegos como en simuladores industriales.
    Separar el tiempo real del tiempo de simulacion es una practica estandar en motores profesionales y sistemas de simulacion industrial.

# 9. Conclusion.
    El modulo Tiempo es una de las piezas fundamentales del nucleo del motor.
    Su corecta implementacion garantiza estabilidad, previsibilidad y control sobre la ejecucion de la simulacion.

    Este modulo esta diseñado para ser simple, independiente y completamente desacoplado del resto de los sistemas, cumpliendo con los principios de buena ingenieria de software.

# 10. Bases computacionales.
    El modulo se apoya en conceptos fundamentales de la arquitectura de computadores y sistemas operativos modernos para garantizar su precision.

#   10.1 Relojes de Hardware y TSC(Time STamp Counter)
    En el nivel mas bajo, el motor aprovecha (a traves de la abstraccion de C++) el contador de ciclos de la CPU. A diferencia del reloj de pared (RTC), que puede tener derivas termicas, el Reloj Monotonico de Debian asegura que los ticks sean constantes, evitando que el motor "viaje al pasado" debido a una sincronizacion de red NTP.

    Cada computadora tiene un cristal de cuarzo que vibra millones de veces por segundo. Debian usa esas vibraciones para contar el tiempo. Al usar steady_clock, el motor se conecta directamente a ese contador fisico que nunca retrocede, dandonos una referencia solida como una roca.

#   10.2 Cuantizacion del Tiempo y Punto Flotante
    La simulacion traduce ticks de nanosegundos (enteros) a segundos (double). Se elige double de 64 bits para minimizar el error de truncamiento en acumulaciones de larga duracion, algo critico en simulaciones industriales donde el motor puede estar encendido durante semanas.

    Una computadora no es una linea recta, es un caos de procesos. Cuando el codigo pide "esperar", Debian puede tardar un poquito mas porque estaba ocupado con otra cosa.
        - El Acumulador de este codigo es como un "amortiguador": absorbe esos baches de tiempo para que una simulacion, por ejemplo petrolera, no sienta los "frenazos" de la computadora.

#   10.3 Latencia del planificador (Scheduling Jitter)
    El diseño reconoce que Debian no es un sistema operativo de tiempo real "duro" por defecto. Al usar un Acumulador, el modulo compensa el jitter del planificador: si el sistema operativo se demora unos milisegundos en devolverle el control al proceso, el acumulador guarda ese tiempo para "pagar la deuda" en el siguiente ciclo, manteniendo la integridad fisica del simulador.

    Precision vs exactitud.
    En el bajo nivel, no nos importa que hora es en el mundo (exactitud), nos importa que cada "segundo" de nuestra simulacion dure exactamente lo mismo(precision). El uso de double y del PASO_FIJO asegura que las leyes de la fisica que programemos no se rompan porque la computadora se puso lenta.

#   10.4 Estabilidad Numerica (Teorema de Nyquist-Shanon aplicado)
    Al imponer un Paso Fijo (Fixed Timestep), garantizamos que la frecuencia de muestreo de la fisica sea superior a la frecuencia de los eventos que queremos simular. Esto evita que las fueras o presiones en el simulador petrolero entren en resonancia y causen valore NaN (Not a number) o infinitos.

    Determinismo:
        Gracias a esta arquitectura, si corres este motor en una PC vieja o en una nueva, el resultado del calculo petrolero sera identico. Esto es vital: en la industria, no podemos permitir que un pozo "explote" en una computadora y en otra no por un error de milisegundos.

        Si vas a programar sobre este motor
