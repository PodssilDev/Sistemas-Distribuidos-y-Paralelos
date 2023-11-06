#include <stdio.h>
#include <omp.h>

int main(){
    int b = 1;

    #pragma omp parallel
    {
        #pragma omp for reduction(*:b)
        for(int i = 0; i < 5; i++){
            b = b + 5;
            printf("Iteracion %d, hebra %d, valor b %d\n", i, omp_get_thread_num(), b);
        }
    }

    printf("b = %d\n", b);
    return 0;
}