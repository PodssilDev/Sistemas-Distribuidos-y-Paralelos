#include <stdio.h>
#include <pmmintrin.h>

int main(){
    __m128 A1, A2, A, B, C, B1, B2, D;
    float a[4] __attribute__((aligned(16))) = { 1.0, 2.0, 3.0, 4.0 };
    float b[4] __attribute__((aligned(16))) = { 0.1, 0.2, 0.3, 0.4 };
    A = _mm_load_ps(a);
    B = _mm_load_ps(b);
    A1 = _mm_moveldup_ps(A);
    A2 = _mm_movehdup_ps(A);
    B1 = _mm_mul_ps(A1, B);
    B2 = _mm_mul_ps(A2, B);
    C = _mm_shuffle_ps(B2, B2, _MM_SHUFFLE(2, 3, 0, 1));
    D = _mm_addsub_ps(B1, C);
    _mm_store_ps(a, D);
    printf("(%f, %f) (%f, %f)\n", a[0], a[1], a[2], a[3]);
    return 0;
}