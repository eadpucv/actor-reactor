# Expo Automata

Servicio Web de visualización de datos de los sensores y disponibilización para [Pure Data](https://puredata.info/).


##### Instrucciones para desabilitar salvapantallas y ahorro de energía


	sudo leafpad ~/.config/lxsession/LXDE-pi/autostart

add:

	@xset s 0 0
	@xset s noblank
	@xset s noexpose
	@xset dpms 0 0 0
	

##### Instrucciones para dejar un archivo autoejecutable al inicio

	leafpad superscript
	
Se crea y se edita el archivo superscript en la carpeta del usuario. El contenido debe ser el siguiente:

	#!/bin/bash
	/home/pi/path-to-app --run

Y en el archivo de inicio de LXDE:

	sudo leafpad ~/.config/lxsession/LXDE-pi/autostart

agregar al final:

	@/home/pi/superscript

