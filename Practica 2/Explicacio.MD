# PRACTICA 2 - VÍCTOR PALLÁS I POL RAICH

## FUNCIONAMENT DE LA PRIMERA PART DE LA PRÀCTICA
Primerament declarem una estructura de boto, en la qual tenim que asignarli un pin de sortida, un nombre de vegades presionat el boto, i si esta presionat el boto.

El "void IRAM_ATTR isr" fa que per cada vegada que s'apreti el boto aumenti en 1 el nombre de vegades que ha estat apretat i posi el estat de el boto en "true".

En el setup asignem el pin, per el qual volem que fagi el "attachInterrupt" i en la manera que volem que el fagi, en aquest cas "FALLING" que fa que s'interrompeixe quan el pin va de "HIGH" a "LOW" .

En el loop detecta si el boto ha estat apretat, donant-se el cas ens mostra per pantalla un misatge dient que el boto ha sigut apretat i el nombre de vegades que ho ha estat, i seguidament canvia el estat del boto a "false" per poder seguir apretant-lo.
Després tenim un altre if que fa que quan hagin passat 1 minut no detecti si el boto esta apretat o no, fent una interrupció al pin, i mostrant per pantalla "Interrupt Detached!". Aquest misatge es mostra cada  minut en cara que el pin ja tingui la interrupció.


Per que no es suma de un en un les vegades que el boto ha estat apretat ?
Aixó ja que quan apretem un boto, aquest fa rebots minimament notables per nosaltres pero que el processador els detecta fent així que sumi d'una sola vegada un increment bastat gran.



## FUNCIONAMENT DE LA SEGONA PART DE LA PRÀCTICA
Primerament declarem unes variables les quals una te que ser "volatile int" la qual esta encarregada de detectar la interrucció, i una variable "int" la qual es el contador de interruccions totals

En el setup 

En el loop detecta si tenim una interrupció, la elimine del contador volatil i li sume al contador total aquesta interrupció, i seguidament pa un "Serial.print" per a poder mostrar text seguit de el nombre total de interrupcións que han hagut.

Per pantalla ens mostrarà una misatge de "An interrupt as occurred. Total number: x" on x es el nombre de interrupcions que porta en total. Aquest misatge apareixera cada segon en el cas d'aquest programa però si vulguesim que aparegues cada 10 (per posar un exemple) simplement necesitariem augmentar el nombre 80 dins el "timerBegin(0, 80, treu)" a "timerBegin(0, 800, true).
Representa que cada segon equival a 80.
Si es modifiques el "timerAlarmWrite()" tambè variaria la velocitat que fa interrupcions ja que lo que fa aquest codi es variar la velocitat en que ho mostrem per pantalla. Com mes petit sigui aquest numero fariem mes interrupcions.