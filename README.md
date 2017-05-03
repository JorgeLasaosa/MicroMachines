-------------------------------------------------------------
							PENGO
-------------------------------------------------------------

Nota: Esta versi�n s�lo es compatible con versiones de OpenGL iguales o superiores a 3.3



-------------------------------------------------------------
				INSTRUCCIONES DE EJECUCI�N
-------------------------------------------------------------

Descargar el fichero zip desde la p�gina de git-hub.
Descomprimir el fichero zip
Acceder al directorio donde se ha descomprimido el archivo.

Ejecutar Pengo.exe dentro del directorio

-------------------------------------------------------------
						COMO JUGAR
-------------------------------------------------------------
El objetivo de pengo es alinear los 3 bloques de diamante y eliminar a sus enemigos, los snobees.

Para ello, pengo puede deslizar bloques que, si encuentran enemigos por delante, los aplastar�n.
Si el bloque de hielo no se puede mover porque tiene otro bloque delante, se romper�.

Tambi�n se pueden eliminar los snobees cuando est�n paralizados pis�ndolos.
Se pueden paralizar de dos formas:
 - Empujando las paredes del exterior, lo cual paralizar� todos los snobees que esten tocando dicha pared.
 - Juntando los 3 bloques de diamante, lo cual paralizar� todos los snobees de la escena.

Cada vez que se elimina a un snobee nace uno nuevo. Los huevos de snobee se reparten al comienzo del nivel,
y se muestra su localizaci�n en color verde durante unos segundos. Si se destruyen estos bloques, tambi�n 
se destruye el huevo de su interior.
Cuando no quedan mas enemigos en el nivel ni huevos de los que nacer, se avanzar� al siguiente nivel.

Si los snobees alcanzan a pengo, se perder� una vida.

Si se consiguen 30000 puntos se obtiene una vida extra.

-------------------------------------------------------------
						CONTROLES
-------------------------------------------------------------

- Tecla acci�n (ctrl): Aceptar en los men�s y empujar los bloques durante el juego.
- Teclas de movimiento (flechas): Desplazarse por los men�s y mover a pengo durante el juego.
- Tecla pause (esc): Pone pause durante el juego.
- Tecla cambio de vista (tab): Cambia a una vista prefijada durante el modo camara (seleccionable desde el men� del juego).
- Click izquierdo del rat�n y arrastrar: Rotar la c�mara 3D durante el juego o el men� de pausa.
- Click derecho del rat�n y arrastrar: Mover la c�mara 3D durante el juego o el men� de pausa.
- Scroll del rat�n: Aumentar/Disminuir zoom. 

Nota: Todas estas teclas son configurables desde el men� principal.