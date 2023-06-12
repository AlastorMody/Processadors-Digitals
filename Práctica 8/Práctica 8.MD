# PRÁCTICA 8

El objetivo de la practica es realizar un bucle de comunicacion de forma que los datos que se manden por el terminal rxd0 se redirijan a la uart 2 txd2 ( que debe estar conectado a rxd2 ) y la recepcion de los datos de la uart2 se reenvien de nuevo a la salida txd0 para que aparezcan en la pantalla del terminal.

Al tener un pin conectado con el otro, el bucle será infinito, y solamente poniendo en el setup un print, este se repetirá indefinidamente en el serial monitor. 

El código es muy sencillo e intuitivo y el montaje también. Por lo tanto con la respuesta esperada en el puerto serie 0, nos quedaremos satisfechos.

## CÓDIGO

~~~~
#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void setup() 
{
  Serial.begin(115200);


  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  
  Serial.println("test");

}

void loop() { 
  while (Serial2.available()) 
  {
    Serial.print(char(Serial2.read()));
  }
}
~~~~

## IMAGEN PUERTO SERIE

![](https://i.ibb.co/nBrMNFC/Puerto-Serie.png)



