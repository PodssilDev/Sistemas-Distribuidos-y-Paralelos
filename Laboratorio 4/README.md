# Laboratorio 4 Sistemas Distribuidos y Paralelos 13329 2-2023
* Fecha de entrega: 20/12/2023

## Descripción:

Este laboratorio consiste en utilizar Python, Kafka y Spark para realizar una comunicación distribuida de datos y luego realizar el procedimiento de grideo sobre datos de visibilidades y obtener la imágen resultante.

El laboratorio es desarrollado principalmente en un Jupyter Notebook, donde se lee el archivo .csv que contiene los datos y estos se envian a tópicos de Kafka. Spark lee los datos desde estos tópicos y a la vez realiza el proceso de grideo, donde los resultados son enviados a 200 archivos .csv los cuales se juntan todos para obtener el archivo .csv de results.csv.

## Requisitos
* [Laboratorio_4_John_Serrano_Sist_Distribuidos_y_Paralelos.ipynb](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%204/Laboratorio_4_John_Serrano_Sist_Distribuidos_y_Paralelos.ipynb), el cual es un archivo de Jupyter Notebook desarrollado en Google Collab y que contiene todas las instalaciones de Kafka y Spark necesarias, y en donde se realiza la lectura y procesamiento de los datos.
* [Python](https://www.python.org/), el cual es necesario para ejecutar el archivo [IFFT.py](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%204/IFFT.py), el cual lee el archivo de results.csv y realiza el arreglo complejo para poder obtener la imágen resultante usando Matplotlib.
* [hltau_completo_uv.csv](https://drive.google.com/file/d/1B3BNwQ4oNM6T-G7mq3radrS15ZuiW15v/view?usp=sharing), el cual es el archivo que contiene los datos a leer y procesar.
* [results.csv](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%204/results.csv) el cual corresponde a los resultasos del proceso de grideo y se obtiene tras ejecutar todo el Jupyter Notebook, y se lee mediante el archivo Python para obtener las imágenes resultantes.

## Resultados

Los resultados de la comunicación distribuida entre Kafka y Spark corresponden al archivo [results.csv](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/blob/main/Laboratorio%204/results.csv)

Al leer este archivo mediante IFFT.py y sacar el arreglo complejo, se obtiene lo siguiente:

* Imágenes resultantes:

![Figure_1](https://github.com/PodssilDev/Sistemas-Distribuidos-y-Paralelos/assets/91446330/7518f708-cdec-41ca-823a-a99a3a3c9492)
