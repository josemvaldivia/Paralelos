#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM 100

__global__ void add (int *a, int *b, int *c)
{
    c[blockIdx.x] = a[blockIdx.x] + b[blockIdx.x];
 
}

void random_ints(int* source, int size)
{
    for (int i=0;i<size;i++)
    {
        source[i]= rand()%100;

    }
}

void show_array(int* source, int size)
{
    for (int i=0;i<size;i++)
    {
        printf("%d\t",source[i]);
    }
}

int main()
{
    srand(time(NULL));
    int* a;
    int* b;
    int* c;

    int* dev_a;
    int* dev_b;
    int* dev_c;

    int size = NUM * sizeof(int);

    cudaMalloc((void**)&dev_a, size);
    cudaMalloc((void**)&dev_b, size);
    cudaMalloc((void**)&dev_c, size);

    a = (int*) malloc(size);
    random_ints(a, NUM);
    b = (int*) malloc(size);
    random_ints(b, NUM);
    c = (int*) malloc(size);

    show_array(a,NUM);

    printf("\n\n");

    show_array(b,NUM);
    cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size, cudaMemcpyHostToDevice);

    add <<< NUM,1 >>> (dev_a, dev_b, dev_c);

    cudaMemcpy(c, dev_c, size, cudaMemcpyDeviceToHost);
    

    printf("Result:\n");
    show_array(c,NUM);
    return 0;
}