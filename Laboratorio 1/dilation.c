#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"


int main(int argc, char *argv[])
{
    // Declaración de variables
    char *inputFileName = NULL;
    char *sequentialOutputFileName = NULL;
    char *simdOutputFileName = NULL;
    int width = 0;
    int opt;
    char type[3];
    int height1;
    int weight1;
    int maxValue;

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
    FILE *inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL)
    {
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }
    // Se lee información sobre la imágen
    fscanf(inputFile, "%s", type);
    fscanf(inputFile, "%d\n", &weight1);
    fscanf(inputFile, "%d\n", &height1);
    fscanf(inputFile, "%d\n", &maxValue);

    // Se crea la variable que contendrá los pixeles de la imágen de entrada
    unsigned char *inputImage = (unsigned char *)malloc(width * width);
    // Se lee la imágen de entrada
    fread(inputImage, sizeof(unsigned char), width * width, inputFile);
    fclose(inputFile);

    // Se crean las imágenes de salida secuencial y paralela
    unsigned char *sequentialOutputImage = (unsigned char *)malloc(width * width);
    unsigned char *simdOutputImage = (unsigned char *)malloc(width * width);

    // Se realiza la dilatación de forma secuencial
    sequentialDilation(inputImage, sequentialOutputImage, width, width);

    // Se realiza la dilatación de forma paralela
    parallelDilation(inputImage, simdOutputImage, width, width);

    // Se crean las imágenes de salida
    writeOutput(sequentialOutputFileName, sequentialOutputImage, width, width);
    writeOutput(simdOutputFileName, simdOutputImage, width, width);

    // Se libera la memoria reservada
    free(inputImage);
    free(sequentialOutputImage);
    free(simdOutputImage);

    return 0;
}