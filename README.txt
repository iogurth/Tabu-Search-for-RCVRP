Nombre: Iván Oyarzún Rojas
Rol: 202073612-k
Paralelo: 201
Ayudante: Diana Gil
Problema: RCVRP

##########################################################

Dentro de cada carpeta, es decir, /debug, /greedy, /Instancias-RCVRP y /lectura-instancias existe un README corto para mejor entendimiento.

##########################################################

Las restricciones del problema RCVRP son cumplidas de forma indirecta dentro de la misma función Greedy generadora.
Esto mismo sucederá para la futura función Tabu Search, por lo que no se necesita un archivo particular restricciones.cpp o un simil.

##########################################################

Muy importante, el archivo instancia.out siempre tendrá un nombre distinto (considerando la instancia que se quiera testear).
Esto hace que se deba eliminar de forma manual el archivo si es que se quiere eliminar.
Si se genera una nueva instancia sin eliminar el .out anterior, el archivo anterior seguirá estando en la carpeta a la altura del main.

##########################################################

Para compilar el programa:

make

Para ejecutar el programa:

./rcvrp

Para limpiar los archivos generados (objetos y ejecutable):

make clean
