# Motor

Es importante señalar que uno de los objetivos del proyecto es la portabilidad total a
cualquier plataforma, poder ejecutarse en una laptop con Debian 13, Windows, Android o iOS,
sin tener que reescribir todo el codigo.

Por esta razon la ley es apegarse a ciertos criterios como el ESTANDAR de C++ (std::chrono, std::string), steady_clock es lo mismo en windows que en linux.

Por consiguiente se debe tener muy en cuenta el:

1. Uso de capas de abstraccion (HAL).
  Usar librerias que traduzcan las funciones del sistema (ej. abrir una ventana) para que funcione igual en windows que en linux.

2. Empaquetado.
  Incluir todas las librerias dentro de la carpeta del ejecutable.

3. Rutas relativas.
  El programa nunca debe buscar archivos en C:\users\nombre, sino en ./Chaos/.

Ademas de por supuesto ser usado en simulaciones industriales de gemelos digitales para la industria de los hidrocarburos

A proposito la clave del exito segun mis investigaciones:

(En construccion)

-----------------------------------------------------------------------------------

# Flujo de objetos

1. El programa principal (Motor) controla el bucle de simulacion.
Un modulo de Tiempo mide intervalos reales entre interacciones 
y entrega un delta de simulacion (en segundos). El Motor acumula
ese delta y ejecuta la logica de la Aplicacion en pasos fijos (fixed step)
para conseguir resultados estables y reproducibles.

2. Componentes del sistema y correspondencia con archivos.

  - Punto de entrada:
    - motor/main.cpp - Crea Motor y arranca ejecutar()

  - Nucleo del motor (sheduler y bucle principal):
    - motor/codigo/nucleo/motor,h (declaraciones)
    - motor/codigo/nucleo/motor.cpp (implementacion del bucle)

  - Modulo Tiempo: mide intervalos, aplica clamp (deltaMaximo), escala
    y pausa:
    - motor/codigo/nucleo/tiempo.h (interfaz)
    - motor/codigo/nucleo/tiempo.cpp (implementacion)

  - Aplicacion (simulacion basica)
    - motor/codigo/nucleo/app.h
    - motor/codigo/nucleo/app.cpp

3. Flujo de ejecucion (paso a paso)

  - main -> crea Motor -> Motor::ejecutar()
  - En cada iteracion:
    I. Motor llama tiempo.actualizar() (que mide con steady_clock cuanto tiempo real paso).
    II. Motor pide tiempo.obtenerDelta() y añade ese valor al acumulador.
    III. Mientras acumulador >= PASO_FIJO (ej. 1/60 s) ejecuta {aplicacion.actualizar(PASO_FIJO)} una o mas veces.
    IV. Resultado: La logica de la aplicacion siempre recibe PASO_FIJO, lo que da estabilidad numerica.

4. Hardware y sistema operativo - que tener en cuenta (nivel hardware/infraestructura)

  - CPU:
    - El motor es CPU-bound cuando hace calculos numericos intensivos. Para un gemelo con modelos complejos, mas nucleos y maor velocidad por nucleo ayudan.
    - Para runs offline, prioriza nuceos rapidos y buena cache. Puedes ejecutar multiples experimentos en paralelo (cada experimento en un proceso distinto) para aprovechar multi core.
  - Memoria (RAM):
    - Modelos con muchos elementos (mallas, estados de planta) consumen RAM. Planifica cantidad segun tamaño de modelo y checkpoints.
  - Almacenamiento:
    - Logs y checkpoints deben ir a disco rapido (SSD). Ej.: guardar periodicamente estados y series temporales.
  - Reloj / Tiempo del sistema:
    - El software usa std::steady_clock (buenas noticias: no depende del reloj de pared). No necesitas sincronizar el reloj del OS para la simulacion.
  - Red:
    - Quiero offline, por eso no depende de red en tiempo real. Para transferencia de datos (cargas, backups) puede usarse medios fisicos o una red aislada.
  - Entorno OS:
    - Linux es una buena opcion para ejecucion de largas simulaciones; facilita scripting, automatizacion y contenedores. Se usa g++ moderno (C++17).
  - Respaldo y redundancia:
    - Se debe configurar backups regulares de checkpoints y del repositorio del codigo.
   
5. ¿A donde ir si algo se rompe?
   - mapa de problemas comunes y a que archivo ir:
  
  - Programa no compila:
       - Revisa mensajes del compilador; suele indicar archivo y linea.
         Archivos principales: motor/main.cpp, motor/codigo/nucleo/*.cpp.
         
    - El motor no arranca / imprime nada:
       - Revisa motor/main.cpp y motor /codigo/nucleo/motor.cpp (inicio del bucle y prints).
         
    - Delta de tiempo incorrecto (saltos grandes o negativos):
      - Revisa delta de tiempo (actualizar, reanudar). Linea relevante: tiempoAnterior = Reloj::now() y el calculo deltaReal = ahora - tiempoAnterior.

    - Pausa/reanudar no funciona:
      - Revisa Tiempo::pausar(), Tiempo::reanudar() en tiempo.cpp.

    - La logica de la simulacion no avanza correctamente:
      - Revisa motor.cpp (acumulador, PASO_FIJO y app.cpp(como usa delta?)).

    - Uso excesivo de CPU:
      - El bucle actual puede consumir mucho CPU. Se esta estudiando añadir un sleep de 1ms en el bucle principal cuando acumulador < PASO_FIJO reduce CPU.

    - Quieres cambiar el PASO_FIJO o deltaMaximo:
      - PASO_FIJO se controla en motor.cpp (const double FPS_OBJETIVO).
      - deltaMaximo esta en tiempo.cpp para imprimir mas o usar una libreria de logging.
