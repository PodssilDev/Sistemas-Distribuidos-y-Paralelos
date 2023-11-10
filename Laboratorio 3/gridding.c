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

void globalMatrixGridding(FILE* inputFile, double delta_u, int N, int chunk_size, int num_tasks, char *output_directory){
    double *matriR = calloc(N*N, sizeof(double));
    double *matriI = calloc(N*N, sizeof(double));
    double *matriW = calloc(N*N, sizeof(double));
    int comp = 0;
    int cont = 0;
    double delta_v = delta_u;

    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int i = 0; i < num_tasks; i++){
                #pragma omp task firstprivate(chunk_size)
                {
                    double u_k, v_k, vr, vi, w, frec, ce; // Variables que se obtienen de la linea
                    double i_k, j_k, calculo_real, calculo_imag, calculo_peso;
                    int cont_linea = 0;
                    while(feof(inputFile) == 0){
                        char readBuffer[256];
                        char readers[chunk_size][256];
                        #pragma omp critical
                        {
                            while(1){
                                fgets(readBuffer, sizeof(readBuffer), inputFile);
                                strcpy(readers[cont_linea], readBuffer);
                                cont_linea += 1;
                                if(cont_linea >= chunk_size) break;
                                if(feof(inputFile)){
                                    comp = 1;
                                    break;
                                }
                            }
                        }
                        
                        for(int j = 0; j < cont_linea; j++){
                            cont += 1;
                            //printf("Linea numero: %d\n ", cont);
                            //printf("%s\n", readers[j]);
                            sscanf(readers[j], "%lf %lf %lf %lf %lf %lf %lf", &u_k, &v_k, &vr, &vi, &w, &frec, &ce);
                            u_k = u_k * (frec / LIGHT_SPEED);
                            v_k = v_k * (frec / LIGHT_SPEED);
                            i_k = round(u_k / delta_u) + (N/2);
                            j_k = round(v_k / delta_v) + (N/2);
                            calculo_real = matriR[(int)i_k*N + (int)j_k] + (vr * w);
                            calculo_imag = matriI[(int)i_k*N + (int)j_k] + (vi * w);
                            calculo_peso = matriW[(int)i_k*N + (int)j_k] + w;
                            #pragma omp critical
                            {
                                matriR[(int)i_k*N + (int)j_k] = calculo_real;
                                matriI[(int)i_k*N + (int)j_k] = calculo_imag;
                                matriW[(int)i_k*N + (int)j_k] = calculo_peso;
                            }
                        }
                        cont_linea = 0;
                    }
                    printf("Se termino de procesar el archivo.\n");
                }
            }
        }
        #pragma omp taskwait
        #pragma omp barrier
        #pragma omp master
        {
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    if(matriW[i*N + j] != 0){
                        matriR[i*N + j] = matriR[i*N + j] / matriW[i*N + j];
                        matriI[i*N + j] = matriI[i*N + j] / matriW[i*N + j];
                    }
                }
            }
            char* output_directory_1 = "datosgrideadosr.raw";
            char* output_directory_2 = "datosgrideadosi.raw";

            FILE *outputFile_1 = fopen(output_directory_1, "wb");
            if(outputFile_1 == NULL){
                fprintf(stderr, "Error al abrir el archivo de salida.\n");
                exit(EXIT_FAILURE);
            }
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    fwrite(&matriR[i*N + j], sizeof(double), 1, outputFile_1);
                }
            }

            fclose(outputFile_1);

            printf("Se termino de escribir el archivo.\n");

            //output_directory_2 = strcat(output_directory_2, "i.raw");

            FILE *outputFile_2 = fopen(output_directory_2, "wb");
            if(outputFile_2 == NULL){
                fprintf(stderr, "Error al abrir el archivo de salida.\n");
                exit(EXIT_FAILURE);
            }
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    fwrite(&matriI[i*N + j], sizeof(double), 1, outputFile_2);
                }
            }
            fclose(outputFile_2);
            printf("Se termino de escribir el archivo.\n");
            free(matriR);
            free(matriI);
            free(matriW);
            fclose(inputFile);
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
    globalMatrixGridding(inputFile, delta_u, N, chunk_size, num_tasks, output_directory);

}