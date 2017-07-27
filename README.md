# Actor Reactor

Este proyecto se basa en la unidad [Actor-Reactor](https://github.com/amereida/actor-re-actor) y se extiende como unidad expositiva multipropósito consistente en:

1. Sensor
2. Procesamiento y disponibilización Web de datos
3. Actuador

La [configuración actual](https://docs.google.com/spreadsheets/d/1H8SptrEivwXtSlFbvr6E0EB0E-gABvcepRMJ9xA1qTE/edit?usp=sharing) se optimiza para la construcción de una exposición cinética en base al movimiento del público. Cada unidad percibe el entorno mediante un sensor de distancia y translada esta intensidad a un motor y a un servicio Web con los datos normalizados para control sonoro mediante [Pure Data](https://puredata.info/).


El actuador, definido mediante un motor stepper, variará su configuración estructural y de transmisión de la energía mecánica, dependiendo de la naturaleza del objeto expuesto.

### Participantes

* [Herbert Spencer](http://wiki.ead.pucv.cl/index.php/Herbert_Spencer), Curatoría
* [Marcelo Araya](http://wiki.ead.pucv.cl/index.php/Marcelo_Araya), Taller de Construcción 4 de Diseño Industrial
* [Leonardo Aravena](http://wiki.ead.pucv.cl/index.php/Leonardo_Aravena), Diseño y fabricación dispositivos
* [Ismael Martinez](https://www.linkedin.com/in/ismael-martinez-8bb78ba3/), Configuración electrónica y servicio Web
* [Óscar Santis](https://soundcloud.com/oscarsantis), Instalación sonora

#### Taller de Construcción 4 de Diseño Industrial

Catalina Vergara, Camila Campos Leiva, Carolina González, Paz Orellana, Javier Andre Arancibia Romero, Javier Zambra, Sofía Soto, Ur Conejeros, Francisca Collarte Videla, Tamara Hidalgo Porzio, Doyma Henríquez Atlagić, María Ignacia Morales, Paula Sotomayor, Javiera Martinez Guajardo, Bastian Maluenda

### Control

Esquema de control de la unidad:

![Esquema de Control](img/actor-reactor-ai.png)


#### Construido sobre

Este proyecto utiliza tecnologías libres de software y hardware abiertos. 

![Arduino](img/logo-arduino.png) ![Raspberry](img/logo-raspi.png) ![Pure Data](img/logo-puredata.png) ![P5js](img/logo-p5js.png) 

