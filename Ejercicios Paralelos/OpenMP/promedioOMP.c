#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


#define N 10
#define H 4

int main(){
    int arr[N];

    // Se llena el arreglo con valores aleatorios 
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100;
    }

    // Imprimir el arreglo
    printf("Arreglo de entrada:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    float promedio = 0;

    #pragma omp parallel num_threads(H)
    {
        #pragma omp for reduction(+:promedio)
        for (int i = 0; i < N; i++) {
            promedio += arr[i];
        }

        #pragma omp single
        {
            promedio = promedio / N;
            printf("Promedio: %f\n", promedio);
        }
    }
    return 0;
}