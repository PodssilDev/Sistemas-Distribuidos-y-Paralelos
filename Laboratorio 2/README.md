# Laboratorio 2 Sistemas Distribuidos y Paralelos 13329 2-2023
* Fecha de entrega: 22/10/2023

## Descripción:

Este laboratorio consiste en utilizar [uC++](https://github.com/pabuhr/uCPP), el cual es una extensión de C++ que permite aplicar mecanismos de concurrencia, para realizar el procedimiento de grideo sobre datos de visibilidades y obtener la imágen resultante.

Se tienen dos soluciones:
* [gridding.cc](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%202/gridding.cc), la cual realiza el proceso de grideo usando matrices globales que son compartidas por todas las tareas.
* [gridding_matriz_propia.cc](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/tree/main/Laboratorio%202), la cual realiza el proceso de grideo donde cada tarea tiene sus propias matrices, las cuales todas se unen al final del proceso.


## Requisitos
* [uC++](https://github.com/pabuhr/uCPP), para ejecutar los archivos .cc, que realizan la lectura y el procesamiento del archivo que contiene los datos.
* [Matlab](https://www.mathworks.com/products/matlab.html), para ejecutar el archivo que permite leer el resultado del proceso de grideo y obtener las imágenes resultantes.
* [hltau_completo_uv.csv](https://drive.google.com/file/d/1B3BNwQ4oNM6T-G7mq3radrS15ZuiW15v/view?usp=sharing), el cual es el archivo que contiene los datos a leer y procesar.

## Resultados

Para ver resultados a detalle, se puede revisar el [informe asociado al laboratorio](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%202/gridding.pdf)

* Imágen resultante con ambas soluciones:

![image](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/assets/91446330/72d3e546-bdb7-4a82-b8cb-07adf9e690be)

## Calificación

* 6.6/7.0
