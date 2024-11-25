# Mandelbrot
---
  
Visualistion de l'ensemble de Mandelbrot pour apprendre le C.
https://fr.wikipedia.org/wiki/Ensemble_de_Mandelbrot

## Utilisation des differentes versions
Les fichiers mandel_v5.c, mandel_v6.c, mandel_v7.c, mandel.c ne sont pas stables donc ne pas les compiler. 
### V[0-4]
Pour les versions 0 à 4 compiler comme ceci. Ensuite des images devraientt soit s'enregistrer soit se créer dans le répértoire. Les images sont dans des formats spéciaux donc il faut un lecteur d'image adapté (par exemple [Okular](https://okular.kde.org/fr/)).

    gcc -W -Wall -ansi -pedantic -o mandel mandel_v#.c -lm
    ./mandel


_mandel_v0 vous affiche direct dans le terminal_
![mandel0](/illustration/mandel0.png)

_mandel_v1/2_
![mandel1](/illustration/mandel1.png)

_mandel_v3 de jolies nuances de blanc et noir_
![mandel3](/illustration/mandel3.png)

_mandel_v4 on zoom_
![mandel4](/illustration/mandel4.png)