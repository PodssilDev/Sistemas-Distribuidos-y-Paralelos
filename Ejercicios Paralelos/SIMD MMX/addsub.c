#include <stdio.h>
#include <pmmintrin.h> // SSE3

int main()
{
    __m128 u, v, w;
    float a[4] __attribute__((aligned(16))) = { 0.1, 0.2, 0.3, 0.4 };
    float b[4] __attribute__((aligned(16))) = { 0.0001, 0.002, 0.003, 0.004 };
    printf("Loading %5.3f %5.3f %5.3f %5.3f into XMM register.\n", a[0], a[1], a[2], a[3] );
    u = _mm_load_ps(a);
    printf("Loading %5.3f %5.3f %5.3f %5.3f into XMM register.\n", b[0], b[1], b[2], b[3] );
    v = _mm_load_ps(b);
    w = _mm_addsub_ps ( u , v);
    _mm_store_ps(b, w);
    printf("Result: %5.3f %5.3f %5.3f %5.3f\n", w[0], w[1], w[2], w[3] );
    return 0;
}