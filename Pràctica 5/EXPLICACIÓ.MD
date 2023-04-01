# Pràctica 5
## Explicació pràctica 5
### Exercici 1
###### Explicació general
El seguent codi busca les adreces dels dispositius que tens conectats i te les mostra per pantalla, escribint "I2C device found at address 0xxx !". 

###### Explicació general
Per compilar el seguent codi necesitem afegir una llibreria nombrada "wire.h".
En el setup trobem un while que espera a que s'inicialitzi el 'serial monitor' i despres escriu en ""I2c scanner".
En el loop trobem que declarem dos variables en format byte, i una altra en enters per tenir el conte total de dispositius. Creem un bucle per poder trobar la adreça dels dispositius.
Finalment escribim un if per veure si trobem algun dispositiu, i posem un 'delay(5000)' que permet esperar 5 segons abans del seguen escaneig.

### Exercici 2
###### Explicació general
