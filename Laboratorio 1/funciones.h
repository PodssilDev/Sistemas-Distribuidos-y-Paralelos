#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <emmintrin.h>

/*
Laboratorio 1 - Sistemas Distribuidos y Paralelos 13329 2-2023
Autor: John Serrano Carrasco
*/

// Cabeceras de las funciones
void parallelDilation(const unsigned char* inputImage, unsigned char* outputImage, int , int );
void sequentialDilation(const unsigned char *inputImage, unsigned char *outputImage, int , int );
FILE* readInput(char *inputFileName, char *type, int, int, int);
void writeOutput(char *nameOutputFile, unsigned char *outputImage, int, int);