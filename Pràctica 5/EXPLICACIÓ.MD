# Pràctica 5
## Explicació pràctica 5
### Exercici 1
##### Explicació general
El seguent codi busca les adreces dels dispositius que tens conectats i te les mostra per pantalla, escribint "I2C device found at address 0xxx !". 

##### Explicació general
Per compilar el seguent codi necesitem afegir una llibreria nombrada "wire.h".
En el setup trobem un while que espera a que s'inicialitzi el 'serial monitor' i despres escriu en ""I2c scanner".
En el loop trobem que declarem dos variables en format byte, i una altra en enters per tenir el conte total de dispositius. Creem un bucle per poder trobar la adreça dels dispositius.
Finalment escribim un if per veure si trobem algun dispositiu, i posem un 'delay(5000)' que permet esperar 5 segons abans del seguen escaneig.

### Exercici 2
##### Explicació general
El seguent codi buscaras les adreces utilitzant el codi anterior, i utilizant-les sabra on te que enviar les dades.Ja que llegirem les temperatures, altura i pressio que ens dona el sensor BMP280 i les mostrarem per pantalla.

##### Explicació codi
En aquest cas tindrem que afegir un parell mes de llibreries, ja que en el nostre cas utilitzarem les llibreries "BMP280" que ens serviran per el sensor de temperatura, presio, i altura. I les llibreries de una pantalla.
En el setup trobarem ames del codi anterior la inicialització de cada dispositiu.
En el loop creem 3 variables i les asignem cada una a una respectiva mesura del sensor "BMP280". seguidament asignarem un tamany de text per la pantalla i on volem que comenci el text en la pantalla, farem un 'display.print()' per escriure el text a la pantalla.
