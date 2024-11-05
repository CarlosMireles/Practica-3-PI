#include "Inc_I2C.h"
// Este algoritmo no es optimo => es muy lento; .... Pero funciona!

void i2c_start() {    // funcion naive para start
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);	// SCL = 1 y SDA = 1
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 	// SCL = 1 y SDA = 1
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);  	// SCL = 1 y SDA = 0 El flanco de bajada causa el start
  digitalWrite(ESC_SCL, LOW );  digitalWrite(ESC_SDA, LOW);  	// SCL = 0 y SDA = 0
}

void i2c_stop() {    // funcion naive para stop 
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW); 	// SCL = 0 y SDA = 0
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW); 	// SCL = 1 y SDA = 0
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);  	// SCL = 1 y SDA = 1 El flanco de subida causa el stop
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);   	// SCL = 1 y SDA = 1
}

void i2c_Ebit1() {    // funcion naive enviar un 1
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH); 	// SCL = 0 y SDA = 1
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 	// SCL = 1 y SDA = 1
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);  	// SCL = 1 y SDA = 1
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH);   	// SCL = 0 y SDA = 1
}

void i2c_Ebit0() {    // funcion naive enviar un 0
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW);  	// SCL = 0 y SDA = 0
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);  	// SCL = 1 y SDA = 0
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);   	// SCL = 1 y SDA = 0
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW);    	// SCL = 0 y SDA = 0
}

void i2c_Ebit(bool val) {    // funcion naive enviar un bit
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, val); 	// SCL = 0 y SDA = val
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, val); 	// SCL = 1 y SDA = val
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, val);  	// SCL = 1 y SDA = val
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, val);   	// SCL = 0 y SDA = val
}

int i2c_Rbit() {        // funcion naive leer un bit
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH); 	// SCL = 0 y SDA = 1
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 	// SCL = 1 y SDA = 1
  digitalWrite(ESC_SCL, HIGH);  int val = digitalRead(LEE_SDA); // Aqui se produce la lectura y se guarda en val!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH);   	// SCL = 0 y SDA = 1
  return val;                                                   
}

void i2c_write_byte(byte ibyte){
   for (int i=0;i<8;i++){
      if ((ibyte&128) != 0) {i2c_Ebit1();} 
      else {i2c_Ebit0();}
      ibyte = ibyte << 1;
   }
}

byte i2c_read_byte(){
     byte ibyte = 0;
     for (int i=0;i<8;i++){
      ibyte= (ibyte<<1)|(i2c_Rbit() & 1);
     }
     return ibyte;
}
