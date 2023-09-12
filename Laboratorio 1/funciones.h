#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <emmintrin.h>

// Cabeceras de las funciones
void parallelDilation(const unsigned char* inputImage, unsigned char* outputImage, int , int );
void sequentialDilation(const unsigned char *inputImage, unsigned char *outputImage, int , int );
void writeOutput(char *nameOutputFile, unsigned char *outputImage, int, int);