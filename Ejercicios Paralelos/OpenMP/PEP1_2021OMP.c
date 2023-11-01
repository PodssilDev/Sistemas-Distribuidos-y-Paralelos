/*
Escriba un codigo en OpenMP donde 5 hebras incrementan en paralelo una variable entera
compartida a, inicializada en cero, y tal que se imprima una sola vez el valor resultante,
antes de salir del bloque paralelo.
*/

#include <stdio.h>
#include <omp.h>

int main() {
    int a = 0; // Variable compartida a
    #pragma omp parallel num_threads(5) // Comienzo de la region paralela con 5 hebras
    {
        #pragma omp for reduction(+:a) // Se realiza un for reduction para que cada hebra incremente a en 1
        for (int i = 0; i < 5; i++) {
                a += 1;
        }
        #pragma omp single // Se imprime el valor de a una sola vez
        printf("a = %d\n", a);
        
    }
}