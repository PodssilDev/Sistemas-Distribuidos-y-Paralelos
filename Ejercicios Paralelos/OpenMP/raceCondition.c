#include <stdio.h>
#include <omp.h>

int main() {
    int yo, vecino, flag[2], buffer[2];
    #pragma omp parallel private(yo, vecino) shared(flag, buffer)
    {
        yo = omp_get_thread_num();
        flag[0] = 0;
        #pragma omp barrier
        buffer[yo] = 5* yo;
        flag[yo] = 1;

        if(yo == 0) {
            vecino = 1;
        } else {
            vecino = 0;
        }
        while(flag[vecino] == 0){

        }
        int result = buffer[vecino] * buffer[yo]; 
    }
}