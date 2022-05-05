/*
* IT 388
* Omar Baloch
* Homework 5
* Matrix addition
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

/* Cuda function that adds two Matrices */ 
__global__ void addMatrices(double *x, double *y, double *z, int n, int m){
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int column = blockIdx.y * blockDim.y + threadIdx.y;
  int index = row*m +column;
  if(row < n && column < m){
    z[index] = x[index] + y[index];
  }
}

/*--------------------------------
 Function to Print Matrices
 --------------------------------*/
void printMatrix(double *x,double *y,double *c, int n, int m) {
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            printf("%.1f ", x[i*n+j]);
        }
        printf("\n");
    }
    printf("+\n");
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            printf("%.1f ", y[i*n+j]);
        }
        printf("\n");
    }
    printf("=\n");
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            printf("%.1f ", c[i*n+j]);
        }
        printf("\n");
    }
}

/*--------------------------------
 Function to measure run time
 --------------------------------*/
double getTime(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec/1000000.0;

}

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {

    double start, elapsed;
    double* z; double* x; double* y;
    int n,m;
    //int blockSize, numBlocks;
    if(argv[1] == 0 || argv[2] == 0 || argv[3] == 0 || argv[4] == 0){
        fprintf(stderr,"\n \t USAGE: <N> <M> <Block Size (x)> <Block Size (y)>\n\n");
        exit(1);
    }
    n=strtol(argv[1], NULL, 10); 
    m=strtol(argv[2], NULL, 10);
    int size = n * m;
    int blockSizeX = strtol(argv[3], NULL, 10);
    int blockSizeY = strtol(argv[4], NULL, 10);
    int numBlocksX = ceil(double(n) / blockSizeX);
    dim3 blockSize=(blockSizeX,blockSizeY);
    dim3 numBlocks = (numBlocksX,1);
    int i,j;
    cudaMallocManaged(&y, size*sizeof(double));
    cudaMallocManaged(&x, size*sizeof(double));
    cudaMallocManaged(&z, size*sizeof(double));

    // generate matrixes
    for (i=0;i<n;++i){
        for(j=0;j<m;++j){
            x[i*m+j]=sin(i*m+j)*sin(i*m+j);
            y[i*m+j]=cos(i*m+j)*cos(i*m+j);

        }
    }
    start = getTime();
    addMatrices<<<numBlocks,blockSize>>>(x, y, z, n, m);
    cudaDeviceSynchronize();
    elapsed = getTime() - start;
    // Print matrices
    if (n<20 && m<20){
        printMatrix(x,y,z,n, m);
    }
    
    printf("Number of blocks: %d, Block size x-direction: %d, Block size y-direction: %d, Matrix size(NxM): %dx%d, Elapsed time:%f\n",numBlocksX,blockSizeX,blockSizeY,n,m,elapsed);
    cudaFree(x); cudaFree(y); cudaFree(z);
    return 0;
}  /* main */