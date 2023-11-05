#include <stdio.h>
#include <stdlib.h>
#include <pmmintrin.h>

int main()
{
    __m128 A1, A2, A, B, C, B1, B2, D;
    float a[4] __attribute__((aligned(16))) = { 1.0, 2.0, 3.0, 4.0 };
    float b[4] __attribute__((aligned(16))) = { 0.1, 0.2, 0.3, 0.4 };
    A = _mm_load_ps(a); //C3 C2 C1 C0
    B = _mm_load_ps(b); //B1 B3 A3 A0
    C = _mm_shuffle_ps(A, B, _MM_SHUFFLE(1, 3, 3, 0));
    _mm_store_ps(a, C);
    printf("%f %f %f %f\n", a[0], a[1], a[2], a[3]);
    D = _mm_shuffle_ps(B, B, _MM_SHUFFLE(2, 1, 1, 3));
    _mm_store_ps(a, D);
    printf("%f %f %f %f\n", a[0], a[1], a[2], a[3]);
    return 0;
}