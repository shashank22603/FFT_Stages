#include <stdio.h>
#include <complex.h>
#include <math.h>

#define N 128 // Adjust the value of N as needed

int main() {
    for (int i=0 ; i<N/2 ;i++){
        printf(" cos(-2*%d * M_PI / %d) + sin(-2*%d * M_PI / %d) * I ,",i,N,i,N);
    }

    return 0;
}
