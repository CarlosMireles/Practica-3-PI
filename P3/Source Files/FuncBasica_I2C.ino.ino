#include "Inc_I2C.h"
// Este algoritmo no es optimo => es muy lento; .... Pero funciona!

void i2c_start() {    // funcion naive para start
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);  // Flanco de bajada =>Esto causa el start
  digitalWrite(ESC_SCL, LOW );  digitalWrite(ESC_SDA, LOW);  // Se pone para asegurarse de deja CLK a 0
}

void i2c_stop() {    // funcion naive para stop 
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);  // Flanco de subida =>Esto causa el stop
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);   // Si se quita sigue funcionando!
}

void i2c_Ebit1() {    // funcion naive enviar un 1
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH);  // Si se quita sigue funcionando!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH);   
}

void i2c_Ebit0() {    // funcion naive enviar un 0
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW);  
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);  
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, LOW);   // Si se quita sigue funcionando!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, LOW);    
}

void i2c_Ebit(bool val) {    // funcion naive enviar un bit
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, val); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, val); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, val);  // Si se quita sigue funcionando!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, val);   
}

int i2c_Rbit() {        // funcion naive leer un bit
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  digitalWrite(ESC_SDA, HIGH); 
  digitalWrite(ESC_SCL, HIGH);  int val = digitalRead(LEE_SDA);  // Aqui se produce la lectura y se guarda en val!
  digitalWrite(ESC_SCL, LOW);   digitalWrite(ESC_SDA, HIGH);   
  return val;                                                      // Aqui se devuelve val!
}

void i2c_esc_byte(byte dato){
   for (int i=0;i<8;i++){
      if ((dato&128) != 0) {i2c_Ebit1();} else {i2c_Ebit0();}
	 dato = dato << 1;
   }
}

byte i2c_lee_byte(){
     byte ibyte = 0;
     for (byte i=0;i<8;i++){
      ibyte= (ibyte<<1)|(i2c_Rbit() & 1);
     }
     return ibyte;
}