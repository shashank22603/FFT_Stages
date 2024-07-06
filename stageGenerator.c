#include <stdio.h>
#include <complex.h>
#include <math.h>

#define N 6 // Adjust the value of N as needed

int main() {
    
 for (int i = 0; i < (1 << (N - 1)); i++) {
        printf("temp%d[i+%d] = temp%d[i+%d] + W%d[%d] * temp%d[i + %d];\n",
               N, i, N - 1, i, 1 << N, i , N - 1, i+ (1 << (N - 1)));
    }
printf("\n\n");
 for (int i = 0; i < (1 << (N - 1)); i++) {
        printf("temp%d[i+%d] = temp%d[i+%d] - W%d[%d] * temp%d[i + %d];\n",
               N, i+ (1 << (N - 1)), N - 1, i, 1 << N, i , N - 1, i+ (1 << (N - 1)));
    }
    return 0;
}