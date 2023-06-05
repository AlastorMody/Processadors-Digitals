# PRÁCTICA 4A
# GENERACIÓN DE UNA PÁGINA WEB

## Funcionamiento

Al incluir las librerias "WiFi.h" y "WebServer.h" podremos utilizar diferentes comandos y expresiones para conectar la micro ESP32 a una red WiFi e instancior un servidor web fácilmente.

El código es sencillo:
 guardamos en dos variables char* el SSID y la contraseña de la red wifi a la que queremos conectarnos. 
 Creamos un objeto WebServer en el puerto 80.
 Guardamos el bloque de código html en una variable String.
 Cuando se inicia el programa, en el monitor serie podemos ir viendo que imprime los pasos que va haciedo tal y como el código del "void setup()" esta hecho. Primero intenta conectar e imprime el SSID de la wifi

