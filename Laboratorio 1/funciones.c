#include "funciones.h"

/*
Entrada: 
    - inputImage: imagen de entrada, de tipo unsigned char *
    - outputImage: imagen de salida, de tipo unsigned char *
    - width: ancho de la imagen, de tipo integer
    - height: alto de la imagen, de tipo integer
Salida: ninguna (void)
Descripción: función para realizar la dilatación de la imagen de forma paralela, utilizando SIMD.
Para ello, se utiliza la función _mm_max_epu8() que permite realizar la operación de máximo entre dos 
registros SIMD. La información de la imágen de entrada inicialmente se carga en arreglos y luego se 
cargan en registros SIMD. Se guarda el resultado en la imagen de salida, cuya variable es outputImage.
*/
void parallelDilation(const unsigned char* inputImage, unsigned char* outputImage, int width, int height){
    // Declaración de variables
    int i, j;
    int max_value = 16;
    int k = 1;
    unsigned char r0[16];
    unsigned char r1[16];
    unsigned char r2[16];
    unsigned char r3[16];
    unsigned char r4[16];
    __m128i R0, R1, R2, R3, R4, result;
    // Se recorre la imagen de entrada desde i = 1 y j = 1 para evitar los bordes
    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {

            // Se calculan los índices de los píxeles vecinos
            int index = i * width + j; // Indice central
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

            // Cuando k= 17, implica que los registros SIMD estarán llenos
            if (k  == max_value) {
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
                // index - 16 para evitar que la imágen se corra a la derecha
                _mm_storeu_si128((__m128i*)(outputImage + index - 16), result);

                // Se reinicia el contador k
                k = 1;
                } 
                else {
                // Si k aún no es igual a 16, el siglo continua. Se aumenta la variable en 1
                k++;
            }
        }
    }
}

/*
Entrada: 
    - inputImage: imagen de entrada, de tipo unsigned char *
    - outputImage: imagen de salida, de tipo unsigned char *
    - width: ancho de la imagen, de tipo integer
    - height: alto de la imagen, de tipo integer
Salida: ninguna (void)
Descripción: función para realizar la dilatación de la imagen de forma secuencial.
Para ello, se recorre la imagen de entrada y se obtiene el máximo de los píxeles vecinos.
Se guarda el resultado en la imagen de salida, cuya variable es outputImage.
*/
void sequentialDilation(const unsigned char *inputImage, unsigned char *outputImage, int width, int height){
    // Declaración de variables
    int i, j;
    // Se recorre la imagen de entrada desde i = 1 y j = 1 para evitar los bordes
    for (i = 1; i < height - 1; i++)
    {
        for (j = 1; j < width - 1; j++)
        {
            // Se declara la variable maxPixel, donde se guardará el pixel de mayor valor
            unsigned char maxPixel = 0;

            // Se recorre desde -1 hasta 1 para obtener los píxeles vecinos
            for (int m = -1; m <= 1; m++)
            {
                for (int n = -1; n <= 1; n++)
                {
                    // Se obtiene el pixel vecino
                    unsigned char pixel = inputImage[(i + m) * width + j + n];
                    // Se compara con el pixel de mayor valor
                    if (pixel > maxPixel)
                    {
                        // Si el pixel vecino es mayor, se actualiza el valor de maxPixel
                        maxPixel = pixel;
                    }
                }
            }
            // Se almacena el resultado en la imagen de salida
            outputImage[i * width + j] = maxPixel;
        }
    }
}

/*
Entrada: 
    - nameOutputFile: nombre del archivo de salida, de tipo char *
    - outputImage: imagen de salida, de tipo unsigned char *
    - width: ancho de la imagen, de tipo integer
    - height: alto de la imagen, de tipo integer
Salida: ninguna (void)
Descripción: función para escribir la imagen de salida en un archivo PGM binario (formato P5).
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