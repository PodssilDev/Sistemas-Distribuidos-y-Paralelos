# Laboratorio 3 Sistemas Distribuidos y Paralelos 13329 2-2023
* Fecha de entrega: 20/11/2023

## Descripción:

Este laboratorio consiste en utilizar [OpenMP](https://www.openmp.org/) en C, el cual es un estandar para programación paralela en sistemas de memoria compartida, para realizar el procedimiento de grideo sobre datos de visibilidades y obtener la imágen resultante.

Se tienen dos soluciones, ambas en el mismo archivo [gridding.c](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%203/gridding.c):
* **globalMatrixGridding**, la cual realiza el proceso de grideo usando matrices globales que son compartidas por todas las tareas.
* **localMatrixGridding**, la cual realiza el proceso de grideo donde cada tarea tiene sus propias matrices, las cuales todas se unen al final del proceso.

## Requisitos
* [OpenMP](https://github.com/pabuhr/uCPP), para ejecutar el archivo de gridding.c, que realiza la lectura y el procesamiento del archivo que contiene los datos. Ambas soluciones retornan dos archivos .raw, cada uno correspondiente a la parte real e imaginaria de los datos. Estos archivos deben ser leidos con el archivo [comprobar_solucion.m](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%203/comprobar_solucion.m). Para usar OpenMP, se debe importar omp.h y al compilar utilizar -fopenmp.
* [Matlab](https://www.mathworks.com/products/matlab.html), para ejecutar el archivo [comprobar_solucion.m](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%203/comprobar_solucion.m) que permite leer los archivos resultantes del proceso de grideo y obtener las imágenes resultantes.
* [hltau_completo_uv.csv](https://drive.google.com/file/d/1B3BNwQ4oNM6T-G7mq3radrS15ZuiW15v/view?usp=sharing), el cual es el archivo que contiene los datos a leer y procesar.

## Resultados

Para ver resultados a detalle, se puede revisar el [informe asociado al laboratorio](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%203/gridding.pdf)

* Imágenes resultantes:
  
![image](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/assets/91446330/c4ae2d95-11a4-4515-b10e-c3ad2ffacd22)

* Gráfico de Speedup con matriz compartida usando un chunk de 1000:

![graf1 (1)](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/assets/91446330/12806bee-84dc-4db9-a3e2-ba773eab6948)

* Gráfico de Speedup con matriz privada usando un chunk de 1000:

![graf2 (1)](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/assets/91446330/a31a1d20-dd1d-44e9-8794-111a447f3eab)



