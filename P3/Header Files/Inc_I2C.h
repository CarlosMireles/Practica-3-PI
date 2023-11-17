  // Declaración de  identificadores de las lÃ­neas dedicadas a I2C (Para usar pinMode() y digitalWrite())  ) 

#define ESC_SCL   4  // puerto de salida para escribir el valor en la lí­nea SCL-out =>IO4 =>PG5
#define ESC_SDA  39  // puerto de salida para escribir el valor en la lí­nea SDA-out =>IO39=>PG2
#define LEE_SCL  40  // puerto de entrada para leer el estado de la lí­nea SCL       =>IO40=>PG1
#define LEE_SDA  41  // puerto de entrada para leer el estado de la lí­nea SDA       =>IO41=>PG0

/*
  // Declaración de  identificadores de las lí­neas dedicadas a I2C (Para usar con DDR, PIN y PORT ) 

#define SCL_OUT 5  // puerto de entrada para leer el estado de la lí­nea SCL_out  =>IO4 =>PG5
#define SDA_OUT 2  // puerto de entrada para leer el estado de la lí­nea SDA_out  =>IO39=>PG2
#define SCL_IN 1   // puerto de salida para escribir el valor en la lí­nea SCL_in =>IO40=>PG1
#define SDA_IN 0   // puerto de salida para escribir el valor en la lí­nea SDA_in =>IO41=>PG0
*/
