#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funciones.h"
#include <getopt.h>

/*
Laboratorio 1 - Sistemas Distribuidos y Paralelos 13329 2-2023
Autor: John Serrano Carrasco
*/

int main(int argc, char *argv[])
{
    // Declaración de variables
    char *inputFileName = NULL;
    char *sequentialOutputFileName = NULL;
    char *simdOutputFileName = NULL;
    int width = 0;
    int opt, height1, weight1, maxValue;
    char type[3];
    unsigned t0_s, t1_s, t0_p, t1_p;
    double time_s, time_p;

    // Se procesan parametros usando getopt
    while ((opt = getopt(argc, argv, "i:s:p:N:")) != -1)
    {
        switch (opt)
        {
        case 'i': // Nombre del archivo de la imágen de entrada
            inputFileName = optarg;
            break;
        case 's': // Nombre del archivo de la imágen de salida secuencial
            sequentialOutputFileName = optarg;
            break;
        case 'p': // Nombre del archivo de la imágen de salida paralela
            simdOutputFileName = optarg;
            break;
        case 'N':
            width = atoi(optarg); // Valor correspondiente al ancho de la imágen
            break;
        default: // Se aborta el programa si se ingresa una opción no valida
            fprintf(stderr, "Uso: %s -i input.pgm -s output_sequential.pgm -p output_simd.pgm -N width\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Se verifica que los argumentos sean válidos
    if (inputFileName == NULL || sequentialOutputFileName == NULL || simdOutputFileName == NULL || width <= 0)
    {
        fprintf(stderr, "Faltan argumentos o argumentos inválidos.\n");
        exit(EXIT_FAILURE);
    }

    // See lee la imágen de entrada desde un archivo PGM binario (formato P5)
    FILE *inputFile = readInput(inputFileName, type, weight1, height1, maxValue);

    // Se crea la variable que contendrá los pixeles de la imágen de entrada
    unsigned char *inputImage = (unsigned char *)malloc(width * width);
    // Se lee la imágen de entrada
    fread(inputImage, sizeof(unsigned char), width * width, inputFile);
    fclose(inputFile);

    // Se crean las imágenes de salida secuencial y paralela
    unsigned char *sequentialOutputImage = (unsigned char *)malloc(width * width);
    unsigned char *simdOutputImage = (unsigned char *)malloc(width * width);

    // Se realiza la dilatación de forma secuencial
    t0_s = clock(); // Inicio del cálculo del tiempo de ejecución
    sequentialDilation(inputImage, sequentialOutputImage, width, width);
    t1_s = clock(); // Fin del cálculo del tiempo de ejecución
    time_s = (double)(t1_s - t0_s) / CLOCKS_PER_SEC; // Cálculo del tiempo de ejecución
    printf("Tiempo secuencial: %f[s]\n", time_s);

    // Se realiza la dilatación de forma paralela
    t0_p = clock(); // Inicio del cálculo del tiempo de ejecución
    parallelDilation(inputImage, simdOutputImage, width, width);
    t1_p = clock(); // Fin del cálculo del tiempo de ejecución
    time_p = (double)(t1_p - t0_p) / CLOCKS_PER_SEC; // Cálculo del tiempo de ejecución
    printf("Tiempo paralelo: %f[s]\n", time_p);

    // Se crean las imágenes de salida
    writeOutput(sequentialOutputFileName, sequentialOutputImage, width, width);
    writeOutput(simdOutputFileName, simdOutputImage, width, width);

    // Se libera la memoria reservada
    free(inputImage);
    free(sequentialOutputImage);
    free(simdOutputImage);
    /*
    // Comprobar que efectivamente el tiempo paralelo es menor al secuencial
    if (time_p < time_s){
        printf("Tiempo correcto\n");
    }
    */
    return 0;
}