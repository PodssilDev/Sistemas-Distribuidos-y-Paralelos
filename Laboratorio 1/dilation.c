#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <emmintrin.h>

void dilateSIMD(const unsigned char* inputImage, unsigned char* outputImage, int width, int height) {
    int i, j;
    int max_value = 16;
    int k = 0;
    unsigned char r0[16];
    unsigned char r1[16];
    unsigned char r2[16];
    unsigned char r3[16];
    unsigned char r4[16];
    __m128i R0, R1, R2, R3, R4, result;

    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {

            // Calcular los índices de los píxeles vecinos
            int index = i * width + j;
            int indexAbove = (i - 1) * width + j;
            int indexBelow = (i + 1) * width + j;
            int indexLeft = i * width + j - 1;
            int indexRight = i * width + j + 1;

            r0[k] = inputImage[indexAbove];
            r1[k] = inputImage[indexLeft];
            r2[k] = inputImage[indexRight];
            r3[k] = inputImage[indexBelow];
            r4[k] = inputImage[index];

            if (k == max_value) {
                // Cargar los píxeles vecinos en registros SIMD
                R0 = _mm_loadu_si128((__m128i*)r0); 
                R1 = _mm_loadu_si128((__m128i*)r1);
                R2 = _mm_loadu_si128((__m128i*)r2);
                R3 = _mm_loadu_si128((__m128i*)r3);
                R4 = _mm_loadu_si128((__m128i*)r4);

                // Aplicar la operación mm_max() para obtener el píxel resultante
                result = _mm_max_epu8(R0, R1);
                result = _mm_max_epu8(result, R2);
                result = _mm_max_epu8(result, R3);
                result = _mm_max_epu8(result, R4);

                // Almacenar el resultado en la imagen de salida
                _mm_storeu_si128((__m128i*)(outputImage + index - 16), result);

                // Reiniciar el contador k
                k = 0;
            } else {
                k++;
            }
        }
    }
}
// Función para realizar la dilatación de la imagen de forma secuencial
void dilateSequential(const unsigned char *inputImage, unsigned char *outputImage, int width, int height)
{
    int i, j;

    for (i = 1; i < height - 1; i++)
    {
        for (j = 1; j < width - 1; j++)
        {
            unsigned char maxPixel = 0;

            // Obtener el máximo de los píxeles vecinos
            for (int m = -1; m <= 1; m++)
            {
                for (int n = -1; n <= 1; n++)
                {
                    unsigned char pixel = inputImage[(i + m) * width + j + n];
                    if (pixel > maxPixel)
                    {
                        maxPixel = pixel;
                    }
                }
            }

            // Almacenar el resultado en la imagen de salida
            outputImage[i * width + j] = maxPixel;
        }
    }
}

int main(int argc, char *argv[])
{
    char *inputFileName = NULL;
    char *sequentialOutputFileName = NULL;
    char *simdOutputFileName = NULL;
    int width = 0;
    int opt;
    char type[3];
    int height1;
    int weight1;
    int maxValue;


    // Procesar los argumentos de línea de comandos usando getopt
    while ((opt = getopt(argc, argv, "i:s:p:N:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            inputFileName = optarg;
            break;
        case 's':
            sequentialOutputFileName = optarg;
            break;
        case 'p':
            simdOutputFileName = optarg;
            break;
        case 'N':
            width = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Uso: %s -i input.pgm -s output_sequential.pgm -p output_simd.pgm -N width\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (inputFileName == NULL || sequentialOutputFileName == NULL || simdOutputFileName == NULL || width <= 0)
    {
        fprintf(stderr, "Faltan argumentos o argumentos inválidos.\n");
        exit(EXIT_FAILURE);
    }

    // Leer la imagen de entrada desde un archivo PGM binario (formato P5)
    FILE *inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL)
    {
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }
    fscanf(inputFile, "%s", type);
    fscanf(inputFile, "%d\n", &weight1);
    fscanf(inputFile, "%d\n", &height1);
    fscanf(inputFile, "%d\n", &maxValue);

    unsigned char *inputImage = (unsigned char *)malloc(width * width);
    fread(inputImage, sizeof(unsigned char), width * width, inputFile);
    fclose(inputFile);
    //printf("%s\n", inputImage);

    // Crear una imagen de salida del mismo tamaño
    unsigned char *sequentialOutputImage = (unsigned char *)malloc(width * width);
    unsigned char *simdOutputImage = (unsigned char *)malloc(width * width);

    // Realizar la dilatación de forma secuencial
    dilateSequential(inputImage, sequentialOutputImage, width, width);

    // Realizar la dilatación usando SIMD
    dilateSIMD(inputImage, simdOutputImage, width, width);

    // Escribir la imagen resultante de forma secuencial en un archivo PGM binario
    FILE *sequentialOutputFile = fopen(sequentialOutputFileName, "wb");
    if (sequentialOutputFile == NULL)
    {
        perror("Error al abrir el archivo de salida secuencial");
        exit(EXIT_FAILURE);
    }
    fprintf(sequentialOutputFile, "P5\n%d %d\n255\n", width, width);
    fwrite(sequentialOutputImage, sizeof(unsigned char), width * width, sequentialOutputFile);
    fclose(sequentialOutputFile);

    // Escribir la imagen resultante usando SIMD en un archivo PGM binario
    FILE *simdOutputFile = fopen(simdOutputFileName, "wb");
    if (simdOutputFile == NULL)
    {
        perror("Error al abrir el archivo de salida SIMD");
        exit(EXIT_FAILURE);
    }
    fprintf(simdOutputFile, "P5\n%d %d\n255\n", width, width);
    fwrite(simdOutputImage, sizeof(unsigned char), width * width, simdOutputFile);
    fclose(simdOutputFile);

    // Liberar la memoria
    free(inputImage);
    free(sequentialOutputImage);
    free(simdOutputImage);

    return 0;
}