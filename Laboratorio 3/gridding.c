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

// Defincion de constantes
const float LIGHT_SPEED = 299792458;
const float PI = 3.14159265358979323846;


/*
Entradas: 
    - inputFile: Archivo de entrada que contiene los datos uvw
    - delta_u: Valor de delta_u
    - N: Tamaño de la matriz
    - chunk_size: Tamaño del chunk
    - num_tasks: Cantidad de tareas
    - output_directory: Nombre del archivo de salida
Salidas: Ninguna (void) aunque se escriben dos archivos de salida y se imprime el tiempo de ejecucion de la sección paralela
Descripcion: Esta funcion realiza el proceso de gridding con matrices globales. Se crean tres una matrices globales que son compartidas 
por todas las las tareas y se realiza el proceso de gridding sobre estas matrices. Al finalizar se escriben los archivos de salida
de la parte real y la parte imaginaria y se imprime el tiempo de ejecucion de la sección paralela.
*/
void globalMatrixGridding(FILE* inputFile, double delta_u, int N, int chunk_size, int num_tasks, char *output_directory){
    // Matriz global de la parte real
    double **matriR = (double **)calloc(N, sizeof(double *));
    for (int i = 0; i < N; i++) {
        matriR[i] = (double *)calloc(N, sizeof(double));
    }

    // Matriz gloabl de la parte imaginaria
    double **matriI = (double **)calloc(N, sizeof(double *));
    for (int i = 0; i < N; i++) {
        matriI[i] = (double *)calloc(N, sizeof(double));
    }

    // Matriz global del peso
    double **matriW = (double **)calloc(N, sizeof(double *));
    for (int i = 0; i < N; i++) {
        matriW[i] = (double *)calloc(N, sizeof(double));
    }

    double delta_v = delta_u; // Se obtiene el valor de delta_v
    double t0 = omp_get_wtime(); // Se registra el tiempo de inicio de la sección paralela
    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int i = 0; i < num_tasks; i++){ // Se crean las tareas
                #pragma omp task
                {
                    // Declaracion de variables privadas para cada tarea
                    double u_k, v_k, w_x, vr, vi, w, frec, ce; 
                    double i_k, j_k, calculo_real, calculo_imag, calculo_peso;
                    int cont_linea = 0;
                    while(feof(inputFile) == 0){ // Mientras no se llegue al final del archivo
                        char readBuffer[256]; // Buffer para leer las lineas del archivo
                        char readers[chunk_size][256]; // Buffer para guardar las lineas del archivo
                        #pragma omp critical // Seccion critica (lectura de archivo)
                        {
                            while(1){
                                fgets(readBuffer, sizeof(readBuffer), inputFile); // Se lee una linea del archivo
                                strcpy(readers[cont_linea], readBuffer); // Se guarda la linea en el buffer
                                cont_linea += 1; // Se aumenta el contador de lineas
                                if(cont_linea >= chunk_size) break; // Si se llega al tamaño del chunk se sale del ciclo
                                if(feof(inputFile)){ // Si se llega al final del archivo se sale del ciclo
                                    break;
                                }
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
                            calculo_real = matriR[(int)i_k][(int)j_k] + (vr * w);
                            calculo_imag = matriI[(int)i_k][(int)j_k] + (vi * w);
                            calculo_peso = matriW[(int)i_k][(int)j_k] + w;
                            #pragma omp critical // Seccion critica (Escritura en matrices compartidas)
                            {
                                // Se escriben los valores calculados en las matrices compartidas
                                matriR[(int)i_k][(int)j_k] = calculo_real;
                                matriI[(int)i_k][(int)j_k] = calculo_imag;
                                matriW[(int)i_k][(int)j_k] = calculo_peso;
                            }
                        }
                        cont_linea = 0; // Se reinicia el contador de lineas
                    }

                }
            }
        }
        #pragma omp taskwait // Se espera a que todas las tareas terminen
        #pragma omp barrier // Se espera a que todas las tareas terminen
        #pragma omp master // Solo la hebra maestra realiza la normalizacion de las matrices
        {
            // Se normalizan las matrices real e imaginaria
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    if(matriW[i][j] != 0){
                        matriR[i][j] = matriR[i][j] / matriW[i][j];
                        matriI[i][j] = matriI[i][j] / matriW[i][j];
                    }
                }
            }
        }
    }
    double t1 = omp_get_wtime(); // Se registra el tiempo de finalizacion de la sección paralela

    double ex_time = t1 - t0; // Se calcula el tiempo de ejecucion de la sección paralela
    printf("Tiempo del proceso de gridding con matriz compartida: %lf [s]\n", ex_time);

    char* output_directory_1 = "datosgrideadosr.raw";
    char* output_directory_2 = "datosgrideadosi.raw";
    
    // Se crea el archivo de salida de la parte real
    FILE *outputFile_1 = fopen(output_directory_1, "wb");
    if(outputFile_1 == NULL){
        fprintf(stderr, "Error al abrir el archivo de salida de la parte real (Matriz global).\n");
        exit(EXIT_FAILURE);
    }
    // Se escriben los valores de la matriz real en el archivo de salida
    for (int i = 0; i < N; i++) {
        fwrite(matriR[i], sizeof(double), N, outputFile_1);
    }
    fclose(outputFile_1); // Se cierra el archivo de salida de la parte real
    printf("Finalizo la escritura del archivo de salida de la parte real (Matriz global).\n");
    
    // Se crea el archivo de salida de la parte imaginaria
    FILE *outputFile_2 = fopen(output_directory_2, "wb");
    if(outputFile_2 == NULL){
        fprintf(stderr, "Error al abrir el archivo de salida de la parte imaginaria (Matriz gloal).\n");
        exit(EXIT_FAILURE);
    }
    // Se escriben los valores de la matriz imaginaria en el archivo de salida
    for (int i = 0; i < N; i++) {
        fwrite(matriI[i], sizeof(double), N, outputFile_2);
    }
    fclose(outputFile_2); // Se cierra el archivo de salida de la parte imaginaria
    printf("Finalizo la escritura del archivo de salida de la parte imaginaria (Matriz global).\n");

    // Liberacion de memoria
    for(int i = 0; i < N; i++){
        free(matriR[i]);
        free(matriI[i]);
        free(matriW[i]);
    }
    free(matriR);
    free(matriI);
    free(matriW);
}


/*
Entradas: 
    - inputFile: Archivo de entrada que contiene los datos uvw
    - delta_u: Valor de delta_u
    - N: Tamaño de la matriz
    - chunk_size: Tamaño del chunk
    - num_tasks: Cantidad de tareas
    - output_directory: Nombre del archivo de salida
Salidas: Ninguna (void) aunque se escriben dos archivos de salida y se imprime el tiempo de ejecucion de la sección paralela
Descripcion: Esta funcion realiza el proceso de gridding con matrices locales. Se crean tres una matrices locales que son privadas
para cada tarea y se realiza el proceso de gridding sobre estas matrices. Al finalizar se escriben los archivos de salida
de la parte real y la parte imaginaria y se imprime el tiempo de ejecucion de la sección paralela.
*/
void localMatrixGridding(FILE* inputFile, double delta_u, int N, int chunk_size, int num_tasks, char *output_directory){
    // Se crea una matriz compartida de la parte real, donde se guardaran los valores de las matrices locales
    double **matriRFinal = (double **)calloc(N, sizeof(double *));
    for (int i = 0; i < N; i++) {
        matriRFinal[i] = (double *)calloc(N, sizeof(double));
    }
    // Se crea una matriz compartida de la parte imaginaria, donde se guardaran los valores de las matrices locales
    double **matriIFinal = (double **)calloc(N, sizeof(double *));
    for (int i = 0; i < N; i++) {
        matriIFinal[i] = (double *)calloc(N, sizeof(double));
    }

    // Se crea una matriz compartida del peso, donde se guardaran los valores de las matrices locales
    double **matriWFinal = (double **)calloc(N, sizeof(double *));
    for (int i = 0; i < N; i++) {
        matriWFinal[i] = (double *)calloc(N, sizeof(double));
    }

    double delta_v = delta_u; // Se obtiene el valor de delta_v
    
    double t0 = omp_get_wtime(); // Se registra el tiempo de inicio de la sección paralela
    #pragma omp parallel
    {
        #pragma omp single
        {
            for(int i = 0; i < num_tasks; i++){ // Se crean las tareas
                #pragma omp task
                {
                    // Cada tarea crea sus propias matrices privadas
                    // Se crea una matriz privada de la parte real
                    double **matriR = (double **)calloc(N, sizeof(double *));
                    for (int i = 0; i < N; i++) {
                        matriR[i] = (double *)calloc(N, sizeof(double));
                    }
                    // Se crea una matriz privada de la parte imaginaria
                    double **matriI = (double **)calloc(N, sizeof(double *));
                    for (int i = 0; i < N; i++) {
                        matriI[i] = (double *)calloc(N, sizeof(double));
                    }
                    // Se crea una matriz privada del peso
                    double **matriW = (double **)calloc(N, sizeof(double *));
                    for (int i = 0; i < N; i++) {
                        matriW[i] = (double *)calloc(N, sizeof(double));
                    }
                    // Declaracion de variables privadas para cada tarea
                    double u_k, v_k, w_x, vr, vi, w, frec, ce; 
                    double i_k, j_k, calculo_real, calculo_imag, calculo_peso;
                    int cont_linea = 0;
                    while(feof(inputFile) == 0){ // Mientras no se llegue al final del archivo
                        char readBuffer[256]; // Buffer para leer las lineas del archivo
                        char readers[chunk_size][256]; // Buffer para guardar las lineas del archivo
                        #pragma omp critical // Seccion critica (lectura de archivo)
                        {
                            while(1){
                                fgets(readBuffer, sizeof(readBuffer), inputFile); // Se lee una linea del archivo
                                strcpy(readers[cont_linea], readBuffer); // Se guarda la linea en el buffer
                                cont_linea += 1; // Se aumenta el contador de lineas
                                if(cont_linea >= chunk_size) break; // Si se llega al tamaño del chunk se sale del ciclo
                                if(feof(inputFile)){ // Si se llega al final del archivo se sale del ciclo
                                    break;
                                }
                            }
                        }
                        
                        for(int j = 0; j < cont_linea; j++){ // Se comienza con el proceso de las lineas leidas
                            // Se guardan los valores de la linea en las variables
                            int result = sscanf(readers[j], "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &u_k, &v_k, &w_x, &vr, &vi, &w, &frec, &ce);
                            u_k = u_k * (frec / LIGHT_SPEED); // Se calcula el valor de u_k
                            v_k = v_k * (frec / LIGHT_SPEED); // Se calcula el valor de v_k
                            i_k = round(u_k / delta_u) + (N/2); // Se calcula el valor de i_k
                            j_k = round(v_k / delta_v) + (N/2); // Se calcula el valor de j_k

                            // Se calculan los valores de la parte real, la parte imaginaria y el peso
                            calculo_real = matriR[(int)i_k][(int)j_k] + (vr * w); 
                            calculo_imag = matriI[(int)i_k][(int)j_k] + (vi * w);
                            calculo_peso = matriW[(int)i_k][(int)j_k] + w;

                            // Se escriben los valores calculados en las matrices privadas
                            matriR[(int)i_k][(int)j_k] = calculo_real;
                            matriI[(int)i_k][(int)j_k] = calculo_imag;
                            matriW[(int)i_k][(int)j_k] = calculo_peso;
                        }
                        cont_linea = 0; // Se reinicia el contador de lineas
                    }
                    #pragma omp critical // Seccion critica (Escritura en matrices compartidas)
                    {
                        // Se suman los valores de las matrices privadas a las matrices compartidas
                        for(int i = 0; i < N; i++){
                            for(int j = 0; j < N; j++){
                                matriRFinal[i][j] += matriR[i][j];
                                matriIFinal[i][j] += matriI[i][j];
                                matriWFinal[i][j] += matriW[i][j];
                            }
                        }
                    }
                }
            }
        }
        #pragma omp taskwait // Se espera a que todas las tareas terminen
        #pragma omp barrier // Se espera a que todas las tareas terminen
        #pragma omp master // Solo la hebra maestra realiza la normalizacion de las matrices
        {
            // Se normalizan las matrices real e imaginaria
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    if(matriWFinal[i][j] != 0){
                        matriRFinal[i][j] = matriRFinal[i][j] / matriWFinal[i][j];
                        matriIFinal[i][j] = matriIFinal[i][j] / matriWFinal[i][j];
                    }
                }
            }
        }
    }
    double t1 = omp_get_wtime(); // Se registra el tiempo de finalizacion de la sección paralela

    double ex_time = t1 - t0; // Se calcula el tiempo de ejecucion de la sección paralela
    printf("Tiempo del proceso de gridding con matriz local: %lf [s]\n", ex_time);

    char* output_directory_1 = "datosgrideadosr_mp.raw";
    char* output_directory_2 = "datosgrideadosi_mp.raw";
    
    // Se crea el archivo de salida de la parte real
    FILE *outputFile_1 = fopen(output_directory_1, "wb");
    if(outputFile_1 == NULL){
        fprintf(stderr, "Error al abrir el archivo de salida de la parte real (Matriz local).\n");
        exit(EXIT_FAILURE);
    }
    // Se escriben los valores de la matriz real en el archivo de salida
    for (int i = 0; i < N; i++) {
        fwrite(matriRFinal[i], sizeof(double), N, outputFile_1);
    }
    fclose(outputFile_1); // Se cierra el archivo de salida de la parte real
    printf("Finalizo la escritura del archivo de salida de la parte real (Matriz local)\n");

    // Se crea el archivo de salida de la parte imaginaria
    FILE *outputFile_2 = fopen(output_directory_2, "wb");
    if(outputFile_2 == NULL){
        fprintf(stderr, "Error al abrir el archivo de salida de la parte imaginaria (Matriz local).\n");
        exit(EXIT_FAILURE);
    }
    // Se escriben los valores de la matriz imaginaria en el archivo de salida
    for (int i = 0; i < N; i++) {
        fwrite(matriIFinal[i], sizeof(double), N, outputFile_2);
    }
    fclose(outputFile_2); // Se cierra el archivo de salida de la parte imaginaria
    printf("Finalizo la escritura del archivo de salida de la parte imaginaria (Matriz local)\n");

    // Liberacion de memoria
    for(int i = 0; i < N; i++){
        free(matriRFinal[i]);
        free(matriIFinal[i]);
        free(matriWFinal[i]);
    }
    free(matriRFinal);
    free(matriIFinal);
    free(matriWFinal);
}


int main(int argc, char *argv[]){
    // Inicializacion de variables
    char *input_file = NULL;
    char *output_directory = NULL;
    double delta_x = 0.0;
    int N = 0;
    int chunk_size = 0;
    int num_tasks = 0;
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
    
    // Se abre el archivo de entrada a usar para el proceso de gridding con matriz global
    FILE* inputFile = fopen(input_file, "r");
    if(inputFile == NULL){
        fprintf(stderr, "Error al abrir el archivo de entrada.\n");
        exit(EXIT_FAILURE);
    }

    // Se abre otro archivo de entrada para el proceso de gridding con matriz local
    FILE* inputFile2 = fopen(input_file, "r");
    if(inputFile2 == NULL){
        fprintf(stderr, "Error al abrir el archivo de entrada.\n");
        exit(EXIT_FAILURE);
    }

    delta_x = (PI/(3600 * 180)) * delta_x; // Se calcula el valor de delta_x en radianes
    double delta_u = 1/(N*delta_x); // Se calcula el delta_u

    // Se llama a la funcion que realiza el proceso de gridding con matriz global
    globalMatrixGridding(inputFile, delta_u, N, chunk_size, num_tasks, output_directory);

    // Se llama a la funcion que realiza el proceso de gridding con matriz local
    localMatrixGridding(inputFile2, delta_u, N, chunk_size, num_tasks, output_directory);

    // Se cierran los archivos de entrada
    fclose(inputFile);
    fclose(inputFile2);
    
    return 0;
}