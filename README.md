# Cuarto_fachada_openGL
# Proyecto 2 de teoría computación grafica e interacción humano computadora
## Notas de optimizacion

* Modelos en formato .
assbin
 reducen el tiempo de carga en un 50%. Si tardaba 10 segundos en cargar con formato .
obj
 ahora lo hace en 5, ademas de que ya no necesita el archivo .
mtl

* Ultima versión de Assimp
 5.0, en esta versión se optimiza el manejo de los mesh con respecto a la versión común 3.2, la única desventaja es que se tuvo que compilar ya que en la página oficial no hay un binario para windows en esa versión
* Texturas en formato png y tamaños cuadrados potencias de 2, largo y ancho iguales.
* Modelos hechos con el estilo low poly. Si se hacen los modelos con baja cantidad de polígonos estos ocuparan menos memoria, pesaran menos y seran mas faciles de texturizar.
