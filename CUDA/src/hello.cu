#include <stdio.h>

__global__ void hello_kernel() {
    printf("Hello from GPU, thread %d\n", threadIdx.x);
}

int main() {
    printf("Hello from CPU\n");
    hello_kernel<<<1, 4>>>();
    cudaDeviceSynchronize();
    return 0;
}
