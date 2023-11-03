#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <getopt.h>
#include <math.h>
#include <string.h>


/*
Laboratorio 3 - Sistemas Distribuidos y paralelos 13329 2-2023
Autor: John Serrano Carrasco
*/

// Definciion de constante
const float LIGHT_SPEED = 299792458;
const float PI = 3.14159265358979323846;

void globalMatrixGridding(FILE* inputFile, double delta_u, int N, int chunk_size, int num_tasks){
    double *matriR = calloc(N*N, sizeof(double));
    double *matriI = calloc(N*N, sizeof(double));
    double *matriW = calloc(N*N, sizeof(double));
    int comp = 0;
    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int i = 0; i < num_tasks; i++){
                #pragma omp task firstprivate(chunk_size)
                {
                    while(comp == 0){
                        char readBuffer[256];
                        char readers[chunk_size][256];
                        #pragma omp critical
                        {
                            for(int j = 0; j < chunk_size; j++){
                                if(fgets(readBuffer, sizeof(readBuffer), inputFile) != NULL){
                                    strcpy(readers[j], readBuffer);
                                }
                                else{
                                    comp = 1;
                                    break;
                                }
                            }
                            for (int j = 0; j < chunk_size; j++) {
                            if (readers[j][0] != '\0') {
                                printf("%s\n", readers[j]);
                            }
                            }
                        };
                        }
                    }
                }
            }
        }
    }

int main(int argc, char *argv[]){
    // Inicializacion de variables
    char *input_file = NULL;
    char *output_directory = NULL;
    double delta_x = 0.0;
    int N = 0;
    int chunk_size = 0;
    int num_tasks = 0;
    unsigned t0, t1;
    double time2;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:d:N:c:t:")) != -1) {
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
            case 't':
                num_tasks = atoi(optarg); // Cantidad de tareas
                break;
            default:
                fprintf(stderr, "Uso: %s -i datosuv.txt -o datosgrideados.raw -d deltax -N tamañoimagen -c chunklectura -t numerotareas\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Se verifican que los argumentos sean validos
    if(input_file == NULL || output_directory == NULL || delta_x <= 0.0 || N <= 0 || chunk_size <= 0 || num_tasks <= 0){
        fprintf(stderr,  "Faltan argumentos o argumentos inválidos.\n");
        exit(EXIT_FAILURE);
    }
    
    FILE* inputFile = fopen(input_file, "r");
    if(inputFile == NULL){
        fprintf(stderr, "Error al abrir el archivo de entrada.\n");
        exit(EXIT_FAILURE);
    }

     delta_x = (PI/(3600 * 180)) * delta_x; // Se calcula el valor de delta_x en radianes
    double delta_u = 1/(N*delta_x); // Se calcula el delta_u
    globalMatrixGridding(inputFile, delta_u, N, chunk_size, num_tasks);

}