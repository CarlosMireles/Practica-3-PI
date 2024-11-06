Práctica 3 
===========
Manejo de una memoria(24LC64) compatible con el bus i2c. Se hará uso de de un menú, disponible por la consola, donde el usuario podrá escribir o leer la memoria. 

***Capacidad de la memoria 8191 bytes***

**El menú muestra las siguientes opciones:**
============================================

1. Guardar un dato (un valor de 0 hasta 255) en cualquier dirección de memoria del dispositivo
2. Leer una posición cualquiera de memoria (máximo 8191)
3. Inicializar un bloque de 256 bytes contiguos a un valor concreto
4. Mostrar el contenido de un bloque de 256 bytes contiguos
5. Inicializar **usando Page Write** un bloque de 256 bytes contiguos
6. Mostrar el contenido de un bloque de 256 **empleando Secuencial Read**
7. Cambiar el banco de memoria

#### Aclaraciones
*PAGE WRITE*: Este método de escritura se basa en lo siguiente, para empezar el maestro envía el byte de control indicando que se va a escribir al esclavo, luego se envía la dirección; parte alta (1 byte) y la parte
baja (1 byte). Luego enviamos 32 bytes con el dato que queremos escribir en memoria. Esto nos permite escribir "páginas" de 32 bytes en memoria, a diferencia de la versión 3, esta versión no desperdicia tiempo enviando 
todo el tiempo la dirección cada vez que se vaya a escribir un byte.

*SECUENCIAL READ*: Este método de lectura comienza con el maestro enviando el byte de control seleccionando el chip de memoria e indicando que se va a escribir, luego pasamos la dirección; parte alta (1 byte) y la parte
baja (1 byte). Posteriormente el maestro envia otro byte de control pero esta vez indicando que se procede a leer. A partir de aqui leemos el byte y el maestro devuelve un 0 al esclavo para indicar que quiere seguir leyendo.
En la versión implementada este proceso se repite 256 veces, para leer un bloque contiguo.
