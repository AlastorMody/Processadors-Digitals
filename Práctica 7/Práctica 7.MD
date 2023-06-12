# PRÁCTICA 7

## EJERCICIO 1 y 2


Nos hemos tomado la libertad de utilizar la WiFi de la ESP32 para conectarnos a internet y reproducir un archivo de audio mp3 desde una URL.

Lo primero que declaramos son los const char* con los datos de nuestra WiFi de casa o nuestro hotspot para que nuestra micro tenga acceso a internet.

Luego declaramos otro const char* con la URL de la canción escogida de cualquier banco de datos de internet.

Creamos los tres objetos de audio que necesitaremos mas adelante, destacar que son punteros, sinó no funcionaría bien.

Hemos escogido un baud rate de 9600 en esta práctica porque el 115200 nos daba problemas en la salida del puerto serie.
Encendemos la WiFi y tenemos un while que intenta conectarse siempre, hasta que lo consiga.

Asignamos al audiLogger la dirección de memoria del puerto Serie, así nos irá chivando información por el mismo.

En la siguientes 6 líneas de código, creamos el objeto con el source del audio utilizando la URL y seguidamente asignamos la ganancia y los pines de la salida de la placa de audio:

~~~~
file = new AudioFileSourceICYStream(URL);
file->SetReconnect(5, 0);
out = new AudioOutputI2S();
out -> SetGain(0.125);
out -> SetPinout(26,25,22);
audio = new AudioGeneratorMP3();
~~~~

Una vez generado el objeto mp3 lo mandamos al output establecido anteriormente con el comando:

audio -> begin(file, out);

En el loop() únicamente hacemos que el audio no pare de reproducirse y esté en loop también.