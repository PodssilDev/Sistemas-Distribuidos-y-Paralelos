#include "funciones.h"

/*
Laboratorio 1 - Sistemas Distribuidos y Paralelos 13329 2-2023
Autor: John Serrano Carrasco
*/

/*
Entrada: 
    - inputImage: imagen de entrada, de tipo unsigned char *
    - outputImage: imagen de salida, de tipo unsigned char *
    - width: ancho de la imagen, de tipo integer
    - height: alto de la imagen, de tipo integer
Salida: Ninguna (void)
Descripción: Función para realizar la dilatación de la imagen de forma paralela, utilizando SIMD.
Para ello, se utiliza la función _mm_max_epu8() que permite realizar la operación de máximo entre dos 
registros SIMD. La información de la imágen de entrada inicialmente se carga en arreglos y luego se 
cargan en registros SIMD. Se guarda el resultado en la imagen de salida, cuya variable es outputImage.
*/
void parallelDilation(const unsigned char* inputImage, unsigned char* outputImage, int width, int height) {
    int i, j;
    int max_value = 15;
    int k = 0;
    unsigned char r0[16];
    unsigned char r1[16];
    unsigned char r2[16];
    unsigned char r3[16];
    unsigned char r4[16];
    __m128i R0, R1, R2, R3, R4, result;

    // Se recorre la imagen de entrada desde i = 1 y j = 1 para evitar los bordes
    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width + 1; j++) {
            // Se calculan los índices de los píxeles vecinos
            int index = i * width + j;         // Indice central
            int indexAbove = (i - 1) * width + j; // Vecino superior
            int indexBelow = (i + 1) * width + j; // Vecino inferior
            int indexLeft = i * width + (j - 1); // Vecino izquierdo
            int indexRight = i * width + (j + 1); // Vecino derecho

            // Se cargan los píxeles vecinos en arreglos
            r0[k] = inputImage[indexAbove];
            r1[k] = inputImage[indexLeft];
            r2[k] = inputImage[indexRight];
            r3[k] = inputImage[indexBelow];
            r4[k] = inputImage[index];

            // Cuando k= 15, implica que los registros SIMD estarán llenos
            if (k == max_value) {
                // Se cargan los píxeles vecinos en registros SIMD
                R0 = _mm_loadu_si128((__m128i*)r0); 
                R1 = _mm_loadu_si128((__m128i*)r1);
                R2 = _mm_loadu_si128((__m128i*)r2);
                R3 = _mm_loadu_si128((__m128i*)r3);
                R4 = _mm_loadu_si128((__m128i*)r4);

                // Se aplica la operación mm_max() para obtener los píxeles resultantes
                result = _mm_max_epu8(R0, R1);
                result = _mm_max_epu8(result, R2);
                result = _mm_max_epu8(result, R3);
                result = _mm_max_epu8(result, R4);

                // Se almacena el resultado en la imágen de salida. Se hace una resta de
                // index - 15 para evitar que la imágen se corra a la derecha
                _mm_storeu_si128((__m128i*)(outputImage + index - 15), result);

                // Se reinicia el contador k
                k = 0;
            } else {
                // Si k aún no es igual a 15, el ciclo continúa. Se aumenta la variable en 1
                k++;
            }
        }
    }
    // Copiar los bordes de la imagen original a la imagen de salida

    // Copiar el borde superior
    for (j = 0; j < width; j++) {
        outputImage[j] = inputImage[j];
    }

    // Copiar el borde inferior
    for (j = 0; j < width; j++) {
        outputImage[(height - 1) * width + j] = inputImage[(height - 1) * width + j];
    }

    // Copiar el borde izquierdo y derecho
    for (i = 1; i < height - 1; i++) {
        outputImage[i * width] = inputImage[i * width];
        outputImage[i * width + width - 1] = inputImage[i * width + width - 1];
    }
}

/*
Entrada: 
    - inputImage: imagen de entrada, de tipo unsigned char *
    - outputImage: imagen de salida, de tipo unsigned char *
    - width: ancho de la imagen, de tipo integer
    - height: alto de la imagen, de tipo integer
Salida: Ninguna (void)
Descripción: Función para realizar la dilatación de la imagen de forma secuencial.
Para ello, se recorre la imagen de entrada y se obtiene el máximo de los píxeles vecinos.
Se guarda el resultado en la imagen de salida, cuya variable es outputImage.
*/
void sequentialDilation(const unsigned char *inputImage, unsigned char *outputImage, int width, int height){
    // Declaración de variables
    int i, j;
    unsigned char pixelUp, pixel, pixelRight, pixelLeft, pixelDown;
    // Se recorre la imagen de entrada desde i = 1 y j = 1 para evitar los bordes
    for (i = 1; i < height - 1; i++)
    {
        for (j = 1; j < width - 1; j++)
        {
            // Se declara la variable maxPixel, donde se guardará el pixel de mayor valor
            unsigned char maxPixel = 0;
            unsigned char pixelArray[5];
            // Se calcula el pixel del centro
            pixel = inputImage[(i * width + j)];
            pixelArray[0] = pixel;
            // Se calcula el pixel de arriba
            pixelUp = inputImage[((i - 1) * width + j)];
            pixelArray[1] = pixelUp;
            // Se calcula el pixel de abajo
            pixelDown = inputImage[((i + 1) * width + j)];
            pixelArray[2] = pixelDown;
            // Se calcula el pixel de la izquierda
            pixelLeft = inputImage[(i * width + (j - 1))];
            pixelArray[3] = pixelLeft;
            // Se calcula el pixel de la derecha
            pixelRight = inputImage[( i * width + (j + 1))];
            pixelArray[4] = pixelRight;
            for(int m = 0; m < 5; m++){
                // Se recorre el arreglo de pixeles para encontrar el mayor
                if (pixelArray[m] > maxPixel)
                {
                    // Si el pixel vecino es mayor, se actualiza el valor de maxPixel
                    maxPixel = pixelArray[m];
                    }
                }
            // Se almacena el resultado en la imagen de salida
            outputImage[i * width + j] = maxPixel;
        }
    }
    for (j = 0; j < width; j++) {
        outputImage[j] = inputImage[j];
    }

    // Copiar el borde inferior
    for (j = 0; j < width; j++) {
        outputImage[(height - 1) * width + j] = inputImage[(height - 1) * width + j];
    }

    // Copiar el borde izquierdo y derecho
    for (i = 1; i < height - 1; i++) {
        outputImage[i * width] = inputImage[i * width];
        outputImage[i * width + width - 1] = inputImage[i * width + width - 1];
    }
}

/*
Entrada: 
    - inputFileName: nombre del archivo de entrada, de tipo char *
    - type: tipo de la imagen, de tipo char *
    - weight: ancho de la imagen, de tipo integer
    - height: alto de la imagen, de tipo integer
    - maxValue: valor máximo de la imagen, de tipo integer
Salida: Puntero al archivo de entrada, de tipo FILE *
Descripción: Función para leer la imagen de entrada desde un archivo PGM binario (formato P5).
Si no se puede abrir el archivo, se muestra un mensaje de error y se aborta el programa.
*/
FILE* readInput(char *inputFileName, char *type, int weight, int height, int maxValue){
    // Se abre el archivo de entrada
    FILE *inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL){ // Se verifica que el archivo se haya creado correctamente
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }
    else{ // Si el archivo se abrió correctamente, se leen los detalles de la imágen
        //printf("Nombre de la Imágen: %s\n", inputFileName);
        fscanf(inputFile, "%s", type);
        //printf("Tipo de la Imágen: %s\n", type);
        fscanf(inputFile, "%d\n", &weight);
        //printf("Ancho de la Imágen: %d\n", weight);
        fscanf(inputFile, "%d\n", &height);
        //printf("Alto de la Imágen: %d\n", height);
        fscanf(inputFile, "%d\n", &maxValue);
        //printf("Valor máximo de la Imágen (0 - 255): %d\n", maxValue);
        return inputFile;
    }
}

/*
Entrada: 
    - nameOutputFile: nombre del archivo de salida, de tipo char *
    - outputImage: imagen de salida, de tipo unsigned char *
    - width: ancho de la imagen, de tipo integer
    - height: alto de la imagen, de tipo integer
Salida: Ninguna (void)
Descripción: Función para escribir la imagen de salida en un archivo PGM binario (formato P5).
*/ 
void writeOutput(char *nameOutputFile, unsigned char *outputImage, int width, int height){
    // Se crea el archivo de salida
    FILE *outputFile = fopen(nameOutputFile, "wb");
    // Se verifica que el archivo se haya creado correctamente
    if (outputFile == NULL)
    {
        perror("Error al abrir el archivo de salida");
        exit(EXIT_FAILURE);
    }
    // Se escribe la información de la imágen de salida
    fprintf(outputFile, "P5\n");
    fprintf(outputFile, "%d %d\n", width, height);
    fprintf(outputFile, "255\n");
    // Se escribe la imágen de salida
    fwrite(outputImage, sizeof(unsigned char), width * height, outputFile);
    fclose(outputFile);
}