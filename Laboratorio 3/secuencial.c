#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <string.h>
#include <time.h>

/*
Laboratorio 3 - Sistemas Distribuidos y Paralelos 13329 2-2023
Autor: John Serrano Carrasco

NOTA: Esta es la version secuencial del programa gridding.c, sin hebras, ni tareas, ni OMP.
Fue creado SOLO para poder comparar los tiempos de ejecucion con la version paralela.

*/

// Defincion de constantes
const float LIGHT_SPEED = 299792458;
const float PI = 3.14159265358979323846;


/*
Entradas: 
    - inputFile: Archivo de entrada que contiene los datos uvw
    - delta_u: Valor de delta_u
    - N: Tamaño de la matriz
    - chunk_size: Tamaño del chunk
    - output_directory: Nombre del archivo de salida
Salidas: Ninguna (void) aunque se escriben dos archivos de salida y se imprime el tiempo de ejecucion de la sección paralela
Descripcion: Esta funcion realiza el proceso de gridding con tres matrices, una para la parte real, otra para la parte imaginaria 
y otra para el peso. Se lee el archivo de entrada y se guardan los valores en las matrices. Luego se normalizan las
matrices y se escriben los valores en dos archivos de salida.
*/
void gridding(FILE* inputFile, double delta_u, int N, int chunk_size, char *output_directory){
    // Matriz de la parte real
    double *matriR = calloc(N*N, sizeof(double));

    // Matriz de la parte imaginaria
    double *matriI = calloc(N*N, sizeof(double));

    // Matriz del peso
    double *matriW = calloc(N*N, sizeof(double));

    double delta_v = delta_u; // Se obtiene el valor de delta_v
    clock_t t0 = clock(); // Se registra el tiempo de inicio del proceso de gridding
    // Declaracion de variables
    double u_k, v_k, w_x, vr, vi, w, frec, ce; 
    double i_k, j_k, calculo_real, calculo_imag, calculo_peso;
    int cont_linea = 0;
    while(feof(inputFile) == 0){ // Mientras no se llegue al final del archivo
        char readBuffer[256]; // Buffer para leer las lineas del archivo
        char readers[chunk_size][256]; // Buffer para guardar las lineas del archivo
        while(1){
            fgets(readBuffer, sizeof(readBuffer), inputFile); // Se lee una linea del archivo
            strcpy(readers[cont_linea], readBuffer); // Se guarda la linea en el buffer
            cont_linea += 1; // Se aumenta el contador de lineas
            if(cont_linea >= chunk_size) break; // Si se llega al tamaño del chunk se sale del ciclo
            if(feof(inputFile)){ // Si se llega al final del archivo se sale del ciclo
                break;
            }
        }

        for(int j = 0; j < cont_linea; j++){ // Se comienza con el proceso de las lineas leidas
            // Se guardan los valores de la linea en las variables
            sscanf(readers[j], "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &u_k, &v_k, &w_x, &vr, &vi, &w, &frec, &ce);
            u_k = u_k * (frec / LIGHT_SPEED); // Se calcula el valor de u_k
            v_k = v_k * (frec / LIGHT_SPEED); // Se calcula el valor de v_k
            i_k = round(u_k / delta_u) + (N/2); // Se calcula el valor de i_k
            j_k = round(v_k / delta_v) + (N/2); // Se calcula el valor de j_k
            // Se calculan los valores de la parte real, la parte imaginaria y el peso
            calculo_real = matriR[(int)i_k * N + (int)j_k] + (vr * w);
            calculo_imag = matriI[(int)i_k * N + (int)j_k] + (vi * w);
            calculo_peso = matriW[(int)i_k * N + (int)j_k] + w;
            // Se escriben los valores calculados en las matrices compartidas
            matriR[(int)i_k * N + (int)j_k] = calculo_real;
            matriI[(int)i_k * N + (int)j_k] = calculo_imag;
            matriW[(int)i_k * N + (int)j_k] = calculo_peso;
                            
            }
        cont_linea = 0; // Se reinicia el contador de lineas
    }
    // Se normalizan las matrices real e imaginaria
    for(int i = 0; i < N*N; i++){
        if(matriW[i] != 0){
            matriR[i] = matriR[i] / matriW[i];
            matriI[i] = matriI[i] / matriW[i];
        }
    }
    clock_t t1 = clock(); // Se registra el tiempo de finalizacion del proceso de gridding

    double ex_time = ((double)(t1 - t0)) / (double)CLOCKS_PER_SEC;
    printf("Tiempo del proceso de gridding: %lf [s]\n", ex_time);

    char* output_directory_1 = "datosgrideadosr.raw";
    char* output_directory_2 = "datosgrideadosi.raw";
    
    // Se crea el archivo de salida de la parte real
    FILE *outputFile_1 = fopen(output_directory_1, "wb");
    if(outputFile_1 == NULL){
        fprintf(stderr, "Error al abrir el archivo de salida de la parte real.\n");
        exit(EXIT_FAILURE);
    }
    // Se escriben los valores de la matriz real en el archivo de salida
    fwrite(matriR, sizeof(double), N*N, outputFile_1);
    fclose(outputFile_1); // Se cierra el archivo de salida de la parte real
    printf("Finalizo la escritura del archivo de salida de la parte real.\n");
    
    // Se crea el archivo de salida de la parte imaginaria
    FILE *outputFile_2 = fopen(output_directory_2, "wb");
    if(outputFile_2 == NULL){
        fprintf(stderr, "Error al abrir el archivo de salida de la parte imaginaria.\n");
        exit(EXIT_FAILURE);
    }
    // Se escriben los valores de la matriz imaginaria en el archivo de salida
    fwrite(matriI, sizeof(double), N*N, outputFile_2);
    fclose(outputFile_2); // Se cierra el archivo de salida de la parte imaginaria
    printf("Finalizo la escritura del archivo de salida de la parte imaginaria.\n");

    // Liberacion de memoria
    free(matriR);
    free(matriI);
    free(matriW);
}


int main(int argc, char *argv[]){
    // Inicializacion de variables
    char *input_file = NULL;
    char *output_directory = NULL;
    double delta_x = 0.0;
    int N = 0;
    int chunk_size = 0;
    int opt;

    while ((opt = getopt(argc, argv, "i:o:d:N:c:")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg; // Nombre del archivo de entrada
                break;
            case 'o':
                output_directory = optarg; // Nombre del archivo de salida
                break;
            case 'd':
                delta_x = atof(optarg); // Tamaño de la celda
                break;
            case 'N':
                N = atoi(optarg); // Tamaño de la matriz
                break;
            case 'c':
                chunk_size = atoi(optarg); // Tamaño del chunk
                break;
            default:
                fprintf(stderr, "Uso: %s -i datosuv.txt -o datosgrideados.raw -d deltax -N tamañoimagen -c chunklectura\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Se verifican que los argumentos sean validos
    if(input_file == NULL || output_directory == NULL || delta_x <= 0.0 || N <= 0 || chunk_size <= 0){
        fprintf(stderr,  "Faltan argumentos o argumentos inválidos.\n");
        exit(EXIT_FAILURE);
    }
    
    // Se abre el archivo de entrada a usar para el proceso de gridding
    FILE* inputFile = fopen(input_file, "r");
    if(inputFile == NULL){
        fprintf(stderr, "Error al abrir el archivo de entrada.\n");
        exit(EXIT_FAILURE);
    }

    delta_x = (PI/(3600 * 180)) * delta_x; // Se calcula el valor de delta_x en radianes
    double delta_u = 1/(N*delta_x); // Se calcula el delta_u

    // Se llama a la funcion que realiza el proceso de gridding
    gridding(inputFile, delta_u, N, chunk_size, output_directory);

    // Se cierran los archivos de entrada
    fclose(inputFile);
    
    return 0;
}