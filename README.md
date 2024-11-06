# Práctica 3: Manejo de una Memoria I2C 24LC64

Este proyecto tiene como objetivo el uso del protocolo I2C para el control de una memoria EEPROM 24LC64. Se implementa un menú de usuario que permite leer y escribir datos en la memoria desde la consola. Se desarrollan funciones para manejar la comunicación I2C directamente desde el Arduino Mega, permitiendo la interacción completa con la memoria.

## Objetivos

1. **Implementar y manejar el protocolo I2C en el Arduino Mega** mediante programación directa, sin el uso de bibliotecas adicionales.
2. **Controlar la memoria EEPROM 24LC64** a través de un menú interactivo, que facilita el acceso a las opciones de lectura y escritura.
3. **Familiarizarse con las operaciones de Page Write y Sequential Read** para el manejo eficiente de bloques de memoria.
4. **Explorar el uso del osciloscopio e I2C Debugger en Proteus** para verificar el correcto funcionamiento de la comunicación I2C.

## Características de la Memoria 24LC64

- **Capacidad**: 8191 bytes (8 Kbytes o 64 kbits).
- **Operación de Page Write**: Escritura en bloques de hasta 32 bytes, lo que permite manejar páginas de datos en una sola operación.
- **Operación de Sequential Read**: Lectura secuencial de bloques de memoria sin necesidad de reenviar la dirección.

## Opciones del Menú de Usuario

El menú de usuario, disponible en la consola serie, proporciona las siguientes opciones:

1. **Guardar un dato**: Permite almacenar un valor entre 0 y 255 en una dirección específica de la memoria.
2. **Leer un dato**: Permite leer el valor almacenado en una dirección específica (rango de 0 a 8191).
3. **Inicializar un bloque de 256 bytes**: Establece un valor concreto en un bloque de 256 bytes contiguos, especificando la dirección de inicio.
4. **Mostrar contenido de un bloque de 256 bytes**: Muestra el contenido de un bloque de 256 bytes contiguos desde una dirección específica.
5. **Inicializar un bloque de 256 bytes usando Page Write**: Realiza la inicialización usando la técnica de Page Write, que mejora la eficiencia.
6. **Mostrar contenido usando Sequential Read**: Muestra el contenido de 256 bytes contiguos utilizando Sequential Read.
7. **Cambiar el banco de memoria**: Permite seleccionar el banco de memoria activo, si es necesario.

## Descripción de las Operaciones

### Page Write

La operación de **Page Write** permite la escritura en bloques de hasta 32 bytes. El flujo es el siguiente:
1. Se envía un byte de control para indicar una operación de escritura.
2. Se envía la dirección completa en dos bytes.
3. Se transmiten hasta 32 bytes de datos, aprovechando el contador interno de la memoria para aumentar la dirección automáticamente. Esto reduce el tiempo de escritura al no requerir el envío de la dirección para cada byte.

### Sequential Read

La operación de **Sequential Read** permite la lectura continua de datos en la memoria:
1. Se envía un byte de control y la dirección de inicio.
2. Se cambia a modo de lectura, y se leen datos secuencialmente.
3. El maestro devuelve un ACK para continuar leyendo o un NO-ACK para finalizar la lectura. Esto permite leer bloques sin necesidad de enviar la dirección en cada ciclo.

## Configuración del Hardware

### Conexiones

- **Arduino Mega**: Utiliza las líneas de propósito general para la simulación del bus I2C:
  - **SCL (PG1, pin 41)** y **SDA (PG0, pin 40)** como líneas de entrada.
  - Para la comunicación, se configuran las líneas SCL (PG5, pin 4) y SDA (PG2, pin 39) como salidas.

### Inicialización del Bus

La configuración del bus incluye:
1. Inicialización de SCL y SDA como entradas/salidas según la dirección de transmisión.
2. Sincronización en la comunicación para establecer la condición de START/STOP.
3. Control del bus para establecer el ACK y NO-ACK en las lecturas y escrituras.

## Implementación del Código

### Operaciones Básicas del Bus I2C

La implementación del protocolo I2C requiere varias funciones básicas para manejar las operaciones de inicio, parada y transferencia de datos:
- **START**: Inicia la transmisión I2C.
- **STOP**: Finaliza la transmisión y libera el bus.
- **Envío de bits**: Funciones para enviar bits de control (0 o 1) al esclavo.
- **ACK y NO-ACK**: Envío y recepción de señales ACK y NO-ACK para continuar o finalizar lecturas.

### Funciones para el Menú de Usuario

Cada opción del menú se implementa en una función específica:
1. **Escribir en la memoria** (`escribir1Byte`): Escribe un valor en una dirección específica.
2. **Leer de la memoria** (`leer1Byte`): Lee y muestra un valor en una dirección especificada.
3. **Inicializar bloque** (`inicializarBloque`): Inicializa un bloque de 256 bytes con un valor específico.
4. **Mostrar bloque** (`leerBloque`):Lee y muestra un bloque de 256 bytes.
5. **Page Write** (`inicializarBloquePageWrite`): Inicializa un bloque de 256 bytes usando Page Write.
6. **Sequential Read** (`leerBloqueSecuencial`): Lee un bloque de 256 bytes usando lectura secuencial.
7. **Cambiar banco de memoria** (`seleccionarBancoMemoria`): Cambia el banco activo para las operaciones de memoria.

## Herramientas de Verificación

Para asegurar el funcionamiento correcto del bus I2C:
- **Osciloscopio en Proteus**: Visualización de señales para verificar la transmisión y la correcta sincronización.
- **I2C Debugger**: Herramienta útil para el análisis detallado de las secuencias de datos, especialmente útil en depuración de funciones complejas como Page Write y Sequential Read.

## Autor
Carlos Mireles Rodríguez
