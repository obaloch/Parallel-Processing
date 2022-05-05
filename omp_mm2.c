/* File: omp_mv.c
 *
 * Purpose:
 *     Computes a parallel matrix-vector product with openMP.
 *      Parallel version 1: parallelizing the outermost loop (i-loop)
 * Linux Servers:
 *  Compile:  gcc -g -Wall -fopenmp -o omp_mm1 omp_mm1.c
 *  Run: ./omp_mm1 <thread_count> <matrix dimension N>
 * Expanse Cluster:
 *  1) load intel compiler
        module load intel mvapich2
    2) compile code with
        icc -o mv omp_mm1.c -qopenmp
    3) submit job script:
        sbatch ompScript.sb
 *
 * IT 388 - Illinois State University
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/** Print Matrix to screen */
void printMatrix(long *C,int N){
    int i,j;
    for ( i = 0; i < N; ++i) {
        for ( j = 0; j < N; ++j) {
            printf("%ld ",C[i*N+j]);
        }
        printf("\n");
    }
}

/* Parallel function */
/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    double start, finish, elapsed;
    long i, j, k, nThreads;
    int N;

    if (argv[1]==0 || argv[2] ==0){
        fprintf(stderr,"\n \t USAGE: <thread_count> <matrix dimension N>\n\n");
        exit(1);
    }

    /* Get number of threads from command line */
    nThreads = atoi(argv[1]);
    omp_set_num_threads(nThreads);
    N = atoi(argv[2]); // get matrix dimension

    //allocates memory for an array of N*N long
    long* A = malloc(N*N*sizeof(long));
    long* B = malloc(N*N*sizeof(long));
    long* C = malloc(N*N*sizeof(long));

    start = omp_get_wtime();

        // generate matrix
        for ( i = 0; i < N; ++i) {
          #pragma omp parallel for
            for ( j = 0; j < N; ++j) {
                A[i*N+j] = (i%2) + j%3;
                B[i*N+j] = (i%3) - j%2;
            }
          }

        // Matrix-Matrix multiplication
        for ( i = 0; i < N; ++i) {
          #pragma omp parallel for
            for ( j = 0; j < N; ++j) {
                C[i*N+j]=0;
                for ( k = 0; k < N; ++k) {
                    C[i*N+j] += A[i*N+k] * B[k*N+j];
                }
            }
          }

    finish = omp_get_wtime();
    elapsed = finish - start;

    printf("Inner(j)loop; Matrix Dimension: C[%d,%d]\tElapsed time: %f sec\nWith N Cores: %ld\n",N,N, elapsed);
    if (N<=20){
        printMatrix(C,N);
        return 0;
      }

    free(A);
    free(B);
    free(C);
    return 0;
}  /* main */
