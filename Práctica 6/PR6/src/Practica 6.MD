# PRÁCTICA 6

## Ejercicio 1

En este apartado utilizaremos el lector de tarjetas microSD para leer un archivo que se encuentra en la raíz de la microSD y nos muestra su contenido de texto por el puerto serie.

Tenemos que incluir las librerias SPI.h y SD.h.
Declaramos un objeto File.

En el setup(), lo primero que nos encontramos es un if() para que nos indique si se ha inicialiado bien el lector de tarjetas. SD.begin(), aquí dentro tenemos que introducir un entero que corresponde al PIN SS de la micro donde esta el lector conectado.

Si se ha inicializado bien el lector, procedemos a abrir el archivo indicando el path correcto: myFile = SD.open("/archivo.txt").
La barra "/" es necesaria para indicar que el archivo se encuentra en la raíz.

El siguiente If-else nos devuelve error si no encuentra el archivo, pero en caso de que lo encuentre el while() escribe todo el contenido del archivo de texto por el puerto serie.

A continuación cerramos el archivo ya que solo podemos tener uno abierto a la vez. Y acaba el programa ya que en el loop() no tenemos nada.

## Ejercicio 2

En este ejercicio incluimos y añadimos las librerías SPI.h y MFRC522.h.

Primero de todo hay que definir los pines de reset y el SS de nuestro lector RFID. A continuación declaramos el objeto pasándole estos parámetros.

En el setup() lo único nuevo es inicializar el Bus SPI y el MFRC522, todo lo demás va en loop().

En el loop() usaremos un if que detecte cuando haya una tarjeta rfid cerca del lector. La función booleana que utilizamos es la siguiente: "mfrc522.PICC_IsNewCardPresent()". 
Después otro if para confirmar que lo que este rfid es válido para lectura, lo sabremos con la expresión: "mfrc522.PICC_ReadCardSerial()". Si el UID es válido, solo queda leerlo y mostrarlo por el puerto serie. Eso lo haremos con un for() que recorra byte por byte el UID leído, lo pase a un valor hexadecimal y lo vaya mostrando por pantalla.

## Ejercicio 3

En este ejercicio haremos uso de los buses SPI para conectar y hacer funcionar dos esclavos a la vez con nuestro maestro. El objetivo es utilizar el lector de microSD y el lector RFID a la vez. Cada vez que el lector RFID detecte un RFID, escribirá su UID en hexadecimal junto la fecha y la hora en un archivo .log que se encuentra en la raíz de la microSD que contiene el lector de microSD.

Para controlar el tiempo hemos añadido además de las librerías de los apartados anteriores, la Time.h y la TimeLib.h.

Lo primero es definir los pines SS de los dos módulos y además el reset de la RC522. 
Creamos un objeto time_h, File, 
