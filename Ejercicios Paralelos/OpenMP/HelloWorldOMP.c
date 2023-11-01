#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel // start parallel region
    {
        int ID = omp_get_thread_num(); // get thread number
        printf("hello(%d)", ID);
        printf("world(%d)\n", ID);
    }
}