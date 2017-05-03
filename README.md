-------------------------------------------------------------
							PENGO
-------------------------------------------------------------

Nota: Esta versión sólo es compatible con versiones de OpenGL iguales o superiores a 3.3



-------------------------------------------------------------
				INSTRUCCIONES DE EJECUCIÓN
-------------------------------------------------------------

Descargar el fichero zip desde la página de git-hub.
Descomprimir el fichero zip
Acceder al directorio donde se ha descomprimido el archivo.

Ejecutar Pengo.exe dentro del directorio

-------------------------------------------------------------
						COMO JUGAR
-------------------------------------------------------------
El objetivo de pengo es alinear los 3 bloques de diamante y eliminar a sus enemigos, los snobees.

Para ello, pengo puede deslizar bloques que, si encuentran enemigos por delante, los aplastarán.
Si el bloque de hielo no se puede mover porque tiene otro bloque delante, se romperá.

También se pueden eliminar los snobees cuando estén paralizados pisándolos.
Se pueden paralizar de dos formas:
 - Empujando las paredes del exterior, lo cual paralizará todos los snobees que esten tocando dicha pared.
 - Juntando los 3 bloques de diamante, lo cual paralizará todos los snobees de la escena.

Cada vez que se elimina a un snobee nace uno nuevo. Los huevos de snobee se reparten al comienzo del nivel,
y se muestra su localización en color verde durante unos segundos. Si se destruyen estos bloques, también 
se destruye el huevo de su interior.
Cuando no quedan mas enemigos en el nivel ni huevos de los que nacer, se avanzará al siguiente nivel.

Si los snobees alcanzan a pengo, se perderá una vida.

Si se consiguen 30000 puntos se obtiene una vida extra.

-------------------------------------------------------------
						CONTROLES
-------------------------------------------------------------

- Tecla acción (ctrl): Aceptar en los menús y empujar los bloques durante el juego.
- Teclas de movimiento (flechas): Desplazarse por los menús y mover a pengo durante el juego.
- Tecla pause (esc): Pone pause durante el juego.
- Tecla cambio de vista (tab): Cambia a una vista prefijada durante el modo camara (seleccionable desde el menú del juego).
- Click izquierdo del ratón y arrastrar: Rotar la cámara 3D durante el juego o el menú de pausa.
- Click derecho del ratón y arrastrar: Mover la cámara 3D durante el juego o el menú de pausa.
- Scroll del ratón: Aumentar/Disminuir zoom. 

Nota: Todas estas teclas son configurables desde el menú principal.