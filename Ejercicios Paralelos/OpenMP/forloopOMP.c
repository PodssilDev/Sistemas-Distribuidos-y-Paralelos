#include <stdio.h>
#include <omp.h>

int main(){

    #pragma omp parallel
    {
        for(int i = 0; i < 9; i++){
            printf("Iteracion %d, hebra %d\n", i, omp_get_thread_num());
        }
    }
    return 0;
}