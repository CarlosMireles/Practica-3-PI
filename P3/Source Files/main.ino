/* Práctica 3: Implementación y uso del protocolo I2C
 * 
 * Descripción: Aplicación correspondiente a la práctica 3 (I2C)
 *
 *
 * Fichero: 	23-24_lab3_base.pdsprj
 * Creado: 		--/10/2022/
 * Autor:		
*/
// Programa de demostración
// Programa para hacer declaración básica
// y enviar un mensaje a la consola
#include "Inc_I2C.h"
#include "I2C_DeclaraFunNaive.h"

// manteniendo lo de la P2 ... 

// Definición para pulsadores
// PORTC: PC[7:0] --> PINES 30-31-32-33-34-35-36-37
#define PRIGHT  30    //	PC[7] pulsador right 
#define PDOWN   31    //	PC[6]		"" down
#define PLEFT   32    //	PC[5]		"" left
#define PENTER 	33    //	PC[4]		"" entrar
#define PUP     34    //	PC[3]		"" up
#define SPEAKER 37    // 	PC[0]		   speaker 

// Definiciones para teclado/cátodos de display (para el manejo de los 4 dígitos del display de forma entrelazada)
// PORTL: PL[7:0] --> PINES 42-43-44-45-46-47-48-49
//														 R1-R2-R3-R4-D1-D2-D3-D4 --> FILAS_TECLADO (R) | CÁTODOS DISPLAY (D)
#define D4    0xFE    // 1111 1110 visualiza 	unidades
#define D3    0xFD    // 1111 1101 		"				decenas
#define D2    0xFB    // 1111 1011 		"				centenas
#define D1    0xF7    // 1111 0111 		"				millares
#define DOFF  0xFF    // 1111 1111 todos apagados: todos los cátados comunes a "1"
#define DON   0xC0    // 1111 0000 todos encendidos: todos los cátodos comunes a "0"
// display
int tabla_7seg[] = {63,06,91,79,102,109,125,39,127,103,0x77,0x7C, 0x39, 0x5E, 0x79,0x71,0xFF}; // 0,1,2... E,F, todos encendidos

	//TECLADO
	// Mapeo matricial para las teclas del teclado
	char teclado_map[][3] = { {'1','2','3'},
                        {'4','5','6'},
                        {'7','8','9'},
                        {'*','0','#'}};
			
int lectura;
int modo=0;

long Dir = 0;
long Dato = 0;

unsigned long time;
boolean isDecimal = false;						

void setup() {
	Serial.begin(9600);			// inicialización del canal serie para comunicarnos con:
												// PC --> "Herramientas/monitor serie" en la plataforma Arduino
												// Proteus --> consola en Proteus (VIRTUAL TERMINAL)
	
  // Inicialización de las líneas dedicadas a I2C (usando pinMode() y digitalWrite())  ) 
  
  // Inicialización de los terminales de entrada
  pinMode(LEE_SDA, INPUT);     
  pinMode(LEE_SCL, INPUT);    
 // Inicialización de los terminales de salida 
  pinMode(ESC_SDA, OUTPUT);     
  pinMode(ESC_SCL, OUTPUT);
 // Para asegurarse de no intervenir (bloquear) el bus, poner SDA _out y SCL _out a "1"....  
  digitalWrite(ESC_SDA, HIGH);
  digitalWrite(ESC_SCL, HIGH);

/*
  // Inicialización de las líneas dedicadas a I2C (usando DDRG y PORTG )
  
  // Inicialización de los terminales de entrada y de salida
  DDRG = 0x24; 		// =B00100100; => terminales de 0,1 de entrada y terminales 2,5 de salida    
 // Para asegurarse de no intervenir (bloquear) el bus, poner SDA _out y SCL _out a "1"....  
  PORTG = 0x24;     // =B00100100; => terminales de 0,1 de entrada-NoPullUp y terminales 2,5 de salida = 1   
*/

 // mantener lo de la P2 ... 

	//Programar PORTA de salida. Conexión a los segmentos a-b-c-d-e-f-g-dp del display 7-segmentos)
	DDRA = 0xFF;    					// También se puede expresar en binario con B11111111 (DDRA = B1111111;)
	
	// Escritura en el PORTA --> Todos los segmentos a "1": encendidos
	PORTA = 0xFF;  					// o también B11111111
	
	// Programación del PUERTO L: filas_teclado(input) | cátodos (output) --> R1-R2-R3-R4-D1-D2-D3-D4
	DDRL = 0x0F;		// PL[7:4] entradas (filas del teclado: R1-R2-R3-R4)
									// PL[3:0] salidas (cátodos comunes display/columnas del teclado: D1-D2/C3-D3/C2-D4/C1
	PORTL = 0xFF; 	// activación resistencias de pull-up en las entradas de las filas de teclado PL[7:4]; display off


	// Programación del PORTC
	DDRC   = B00000001; // 0x01:Todos los bits de entrada menos PC0 que es de salida (speaker)
	PORTC = B11111110;	// 0xFE: activación de todas las resistencias de pull-up excepto la de PC0  
	 
	delay(150);
	mostrarMenu();
	
}

void mostrarMenu(){
		Serial.println("--------MENU DE OPCIONES--------");
		Serial.println("OPCIÓN 1 - Guardar un dato en memoria (0-255)");
		Serial.println("OPCIÓN 2 - Leer una posición de memoria");
		Serial.println("OPCIÓN 3 - Inicializar un bloque de memoria (256 bytes) a un cierto valor (0-255)");
		Serial.println("OPCIÓN 4 - Mostrar el contenido de un bloque de memoria comenzando en una dirección específica");
		Serial.println("OPCIÓN 5 - Inicializar un bloque de memoria (256 bytes) a un cierto valor (0-255) utilizando 'Page Write'");
		Serial.println("OPCIÓN 6 - Mostrar el contenido de un bloque de memoria comenzando en una dirección específica utilizando 'Secuencial Read'");
		Serial.println("OPCIÓN 7 - Mostrar Menú");
		Serial.println("");
		Serial.println("");
}

// Funcion que muestra la opción seleccionada por pantalla
void mostrarOpcion(String s){
		Serial.println("Se ha seleccionado la opción: " + String(modo));
		Serial.println(s);
		Serial.println("");
		modo = 0;
}

// Función para obtener la dirección por pantalla
long obtenerDireccion(String s, int n){
		Serial.println(s);
		delay(1000);
		long numero = Serial.parseInt();
		Serial.println("Dirección de memoria: " + String(numero));
		while (numero > n) {
			Serial.println(s);
			delay(1000);
			numero = Serial.parseInt();
			Serial.println("Dirección de memoria: " + String(numero));
			delay(1000);
		}
		Serial.println("");
		return numero;
}

// Función para pedir el dato por pantalla
long obtenerDato(String s, int n){
		Serial.println(s);
		delay(1000);		
		long dato = Serial.parseInt();
		Serial.println("Dato: "  + String(dato));
		
		while (dato > n){
			Serial.println(s);
			delay(1000);
			dato = Serial.parseInt();
			Serial.println("Dato: " + String(dato));
			delay(1000);
		}
		Serial.println("");
		return dato;
}

// Devuelve una string para imprimir un numero hexadecimal en el formato:  0x00
String convertirAHexadecimal(long valor){
		if (valor < 10 ){
			return "0x0" + String(valor, HEX);
		}
		return "0x" + String(valor, HEX);
}

// Devuelve una string para imprimir un numero decimal en el formato:  000 (Ejemplo 56 = 056)
String convertirADecimal(long valor){
		if (valor < 10 ) return "00" + String(valor);
		if (valor < 100) return "0" + String(valor);
		return String(valor);
}

// Pide por pantalla al usuario si tiene que mostrar el bloque en hexadecimal o decimal
boolean pedirUnidad(){
		comienzo:
			Serial.println("Seleccione en que unidades quiere visualizar los bytes de la memoria");
			Serial.println("            0 - Hexadecimal            /            1 - Decimal            ");
			Serial.println("");
			delay(1500);		
		
			long decimal = Serial.parseInt();
			Serial.println("Se ha seleccionado: "  + String(decimal));

		if (decimal == 0) return false;
		if (decimal == 1) return true;
		goto comienzo;
}

// Funcion que comprueba si la dirección es múltiplo de 32
long direccionCompatible(long direccionAComprobar){
		long direccion = direccionAComprobar;
		while (direccion % 32 != 0){
				Serial.println("Introduzca una dirección múltiplo de 32. Ejemplo: 32, 64, 96, 128....");
				delay(1500);
				direccion = Serial.parseInt();
				Serial.println("Dirección: "  + String(direccion));
		}
		return direccion;
}

void loop() {

	 if (Serial.available() > 0) {
		  lectura = Serial.read() - 0x30;
		  modo = lectura;
	 }

	switch (modo){
		  case 0:
				break;
		  case 1:
				mostrarOpcion("Guardar un byte en memoria");
				Dir = obtenerDireccion("Por favor introduzca una dirección válida (0-8191)", 8191);
				Dato = obtenerDato("Introduzca un dato válido entre 0 y 255", 255);
				escribir1Byte(Dir, Dato, true);
				break;
		  case 2:
				mostrarOpcion("Leer un byte de la memoria");
				Dir = obtenerDireccion("Por favor introduzca una dirección válida (0-8191)", 8191);
				leer1Byte(Dir, true);
				break;
		  case 3:
				mostrarOpcion("Inicializar un bloque de 256 bytes");
				Dir = obtenerDireccion("Introduzca la dirección del primer elemento: entre 0 y 8191", 8191);
				Dato = obtenerDato("Introduzca un dato válido entre 0 y 255", 255);
				
				time = inicializarBloque(Dir, Dato);
				
				Serial.println("Tiempo total = " + String(time) + " milisegundos");
				Serial.println("");
				break;		  
		  case 4:
				mostrarOpcion("Leer un bloque de 256 bytes");
				Dir = obtenerDireccion("Introduzca la dirección del primer elemento: entre 0 y 8191", 8191);
				isDecimal = pedirUnidad();
				
				time = leerBloque(Dir, isDecimal);

				Serial.println("Tiempo total = " + String(time) + " milisegundos");
				Serial.println("");
				break;
		  case 5:
				mostrarOpcion("Inicializar un bloque de 256 bytes empleando 'Page Write'");
				Dir = obtenerDireccion("Introduzca la dirección del primer elemento: entre 0 y 8191", 8191);
				Dir = direccionCompatible(Dir);
				Dato = obtenerDato("Introduzca un dato válido entre 0 y 255", 255);

				time = inicializarBloquePageWrite(Dir, Dato);
				
				Serial.println("Tiempo total = " + String(time) + " milisegundos");
				Serial.println("");
				break;		  
		  case 6:
				mostrarOpcion("Leer un bloque de 256 bytes utilizando 'Secuencial Read'");
				Dir  = obtenerDireccion("Introduzca la dirección del primer elemento: entre 0 y 8191", 8191);

				isDecimal = pedirUnidad();
				
				time = leerBloqueSecuencial(Dir, isDecimal);

				Serial.println("Tiempo total = " + String(time) + " milisegundos");
				Serial.println("");
				break;
		  case 7:
				mostrarMenu();
				modo = 0;
				break;

	}
}
  
  
void escribir1Byte(long dir, long dato, boolean mostrar){
		start:
			i2c_start();
	    
			i2c_esc_byte(0xA0); // chip 0 y seleccionamos escritura
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			// Mandar la direccion en la que se va a escribir
			i2c_esc_byte(dir >> 8); // te quedas con los mas significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			i2c_esc_byte(dir & 0xFF); // te quedas con los menos significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			// Dato a escribir
			i2c_esc_byte(dato);
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			i2c_stop();
			
			if (mostrar){
					Serial.println("Se ha escrito el dato " + String(dato) + " (HEX: "  + convertirAHexadecimal(dato) + ")" + " en la posición de memoria: " + String(dir));
					Serial.println("");
			}
}


byte leer1Byte(long dir, boolean mostrar){
		start:
			i2c_start();
	    
			i2c_esc_byte(0xA0); // chip 0 y seleccionamos escritura
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			// Mandar la direccion en la que se va a leer
			i2c_esc_byte(dir >> 8); // te quedas con los mas significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			i2c_esc_byte(dir & 0xFF); // te quedas con los menos significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
			
			i2c_start();
			
			i2c_esc_byte(0xA1); // chip 0 y seleccionamos lectura
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
			
			// Leemos el byte de la memoria 
			byte datoLeido = i2c_lee_byte();
	
			i2c_stop();

			if (mostrar){
					Serial.println("De la posición de memoria: " + String(dir) + " se ha leido el valor " + String(datoLeido) + " (HEX: "  + convertirAHexadecimal(datoLeido) + ")");
					Serial.println("");
			}
			return datoLeido;
}  


long inicializarBloque(long dir, long dato){
			long dirInicial = dir;
			Serial.println("Escribiendo...");
			Serial.println("");
			unsigned long timeIB = millis();
			
			for (int i=0;i<256;i++){
					if (dir > 8191) break;
					escribir1Byte(dir, dato, false);
					dir++;
			}
			
			timeIB = millis() - timeIB;
			Serial.println("Hecho! Se ha copiado en el bloque " + String(dirInicial) + "-" + String(dir-1) + " el valor " + String(dato) + " (HEX: "  + convertirAHexadecimal(dato) + ")");
			return timeIB;
}


// Funcion para mostrar las columnas cuando se muestra el bloque
void ponerCabecera(boolean decimal){
			if (decimal){
					Serial.println("         0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15");
					Serial.println("------------------------------------------------------------------------");
			}
			else{
					Serial.println("          0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15");
					Serial.println("----------------------------------------------------------------------------------------");
			}
}


// Funcion para mostrar los indices a la hora de mostrar el bloque leido 
void ponerIndices(int indice){
			if (indice % 16 != 0) return;
			if (indice == 0) Serial.print(convertirADecimal(indice) + ")   ");
			else {
					Serial.println("");
					Serial.print(convertirADecimal(indice) + ")   ");
			}
}


long leerBloque(long dir, boolean decimal){
			byte datoLeido = 0;
			long dirInicial = dir;
			Serial.println("Leyendo...");
			Serial.println("");
			
			ponerCabecera(decimal);
				
			unsigned long timeLB = millis();
	
			for (int i=0;i<256;i++){
					ponerIndices(i);
					if (dir > 8191) break;
					datoLeido = leer1Byte(dir, false);

					if (decimal) Serial.print(convertirADecimal(datoLeido) + " ");
					if (!decimal) Serial.print(convertirAHexadecimal(datoLeido) + " ");
					dir++;
			}

			timeLB = millis() - timeLB;
			Serial.println("");
			Serial.println("Hecho! Se ha mostrado el contenido del bloque " + String(dirInicial) + "-" + String(dir-1));
			return timeLB;
}


void escribirPagina(long dir, long dato){

		start:
			i2c_start();
	    
			i2c_esc_byte(0xA0); // chip 0 y seleccionamos escritura
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			// Mandar la direccion en la que se va a escribir
			i2c_esc_byte(dir >> 8); // te quedas con los mas significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			i2c_esc_byte(dir & 0xFF); // te quedas con los menos significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez

			for (int i=0;i<32;i++){
					i2c_esc_byte(dato); // chip 0 y seleccionamos lectura
					if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez 
			}
			i2c_stop();
}


long inicializarBloquePageWrite(long dir, long dato){
			long dirInicial = dir;

			Serial.println("Escribiendo...");
			Serial.println("");				
			unsigned long timeIBPR = millis();

			for (int i=0;i<8;i++){
					if (dir > 8191) break;
					escribirPagina(dir, dato);
					dir = dir + 32;
			}
			
			timeIBPR = millis() - timeIBPR;

			Serial.println("Hecho! Se ha copiado en el bloque " + String(dirInicial) + "-" + String(dir) + " el valor " + String(dato) + " (HEX: "  + convertirAHexadecimal(dato) + ")");
			return timeIBPR;
}


// Función para mostrar en pantalla el array leido con la opción de secuencial real
void mostrarArray(byte array[], boolean decimal, long rango){
			if (decimal){
					for (int i=0;i<rango;i++){
							if (i==0){
								Serial.print(convertirADecimal(i) + ")   " + convertirADecimal(array[i]) + " ");
								continue;
							}
							if (i%16 == 0){
								Serial.println("");
								Serial.print(convertirADecimal(i) + ")   " + convertirADecimal(array[i]) + " ");
								continue;
							}
							Serial.print(convertirADecimal(array[i]) + " ");
					}
			}
			else{
					for (int i=0;i<rango;i++){
							if (i==0){
								Serial.print(convertirADecimal(i) + ")   " + convertirAHexadecimal(array[i]) + " ");
								continue;
							}
							if (i%16 == 0){
								Serial.println("");
								Serial.print(convertirADecimal(i) + ")   " + convertirAHexadecimal(array[i]) + " ");
								continue;
							}
							Serial.print(convertirAHexadecimal(array[i]) + " ");							
					}
			}
}


long calcularMinimo(long numeroLecturas){
		if (numeroLecturas > 256) return 256;
		return numeroLecturas;
}


long leerBloqueSecuencial(long dir, boolean decimal){			

		start:
			long numeroLecturas = 256;
			numeroLecturas  = calcularMinimo(8191 - dir);
			byte bloqueLeido[256];
		
			unsigned long timeLBS = millis();

			i2c_start();
	    
			i2c_esc_byte(0xA0); // chip 0 y seleccionamos escritura
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			// Mandar la direccion en la que se va a leer
			i2c_esc_byte(dir >> 8); // te quedas con los mas significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
	    
			i2c_esc_byte(dir & 0xFF); // te quedas con los menos significativos y se mandan para el bus
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez
			
			i2c_start();
			
			i2c_esc_byte(0xA1); // chip 0 y seleccionamos lectura
			if (i2c_Rbit() != 0) goto start; // si ACK devuelve 1 nadie respondio entncs al start otra vez						

			for (int i=0;i<numeroLecturas;i++){
					bloqueLeido[i] = i2c_lee_byte();
					i2c_Ebit0();
			}
			i2c_Ebit1();
			i2c_stop();

			timeLBS = millis() - timeLBS;
			Serial.println("Leyendo...");
			Serial.println("");	
			ponerCabecera(decimal);
			mostrarArray(bloqueLeido, decimal, numeroLecturas);
			
			Serial.println("");
			Serial.println("Hecho! Se ha mostrado el contenido del bloque " + String(dir) + "-" + String(dir+numeroLecturas));
			return timeLBS;
}

