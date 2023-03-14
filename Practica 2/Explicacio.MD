# PRACTICA 2 - VÍCTOR PALLÁS I POL RAICH

## FUNCIONAMENT DE LA PRIMERA PART DE LA PRÀCTICA
Primerament declarem una estructura de ó, en la qual tenim que assignar-li un pin de sortida, un nombre de vegades pressionat el botó, i si esta pressionat el boto.

El "void IRAM_ATTR isr" fa que per cada vegada que es premi el botó augmenti en 1 el nombre de vegades que ha estat premut i posi el estat de el botó en "true"

En el setup assignem el pin, per el qual volem que faci el "attachInterrupt" i en la manera que volem que el faci, en aquest cas "FALLING" que fa que s'interrompi quan el pin va de "HIGH" a "LOW" .

En el loop detecta si el boto ha estat premut, donant-se el cas ens mostra per pantalla un missatge dient que el boto ha sigut premut i el nombre de vegades que ho ha estat, i seguidament canvia el estat del boto a "false" per poder seguir premem-lo.
Després tenim un altre if que fa que quan hagin passat 1 minut no detecti si el boto esta premut o no, fent una interrupció al pin, i mostrant per pantalla "Interrupt Detached!". Aquest missatge es mostra cada  minut en cara que el pin ja tingui la interrupció.

Per que no es suma de un en un les vegades que el botó ha estat premut
Això ja que quan premem un botó, aquest fa rebots mínimament notables per nosaltres però que el processador els detecta fent així que sumi d'una sola vegada un increment bastat gran.


## FUNCIONAMENT DE LA SEGONA PART DE LA PRÀCTICA
Primerament declarem unes variables les quals una te que ser "volatile int" la qual esta encarregada de detectar la interrupció, i una variable "int" la qual es el comptador de interrupcions totals

En el setup 

En el loop detecta si tenim una interrupció, la elimina del comptador volàtil i li suma al comptador total aquesta interrupció, i seguidament pa un "Serial.print" per a poder mostrar text seguit de el nombre total de interrupcions que han hagut.

Per pantalla ens mostrarà una missatge de "An interrupt as occurred. Total number: x" on x es el nombre de interrupcions que porta en total. Aquest missatge apareixerà cada segon en el cas d'aquest programa però si volguéssim que aparegués cada 10 (per posar un exemple) simplement necessitaríem augmentar el nombre 80 dins el "timerBegin(0, 80, treu)" a "timerBegin(0, 800, true).
Representa que cada segon equival a 80.
Si es modifiques el "timerAlarmWrite()" també variaria la velocitat que fa interrupcions ja que lo que fa aquest codi es variar la velocitat en que ho mostrem per pantalla. Com mes petit sigui aquest numero faríem mes interrupcions.
