/* File: parallel_mm.c
* Omar Baloch
* Homework 3
*
* Purpose:  Implement parallel matrix-matrix multiplication using
*           one-dimensional arrays to store the vectors and the
*           matrix.  The matrix is distributed by block rows.
*           Broadcast to all processes.
*
* Compile:  mpicc -g -Wall -o parallel_mm parallel_mm.c
* Run:      mpiexec -n <number of processes> ./parallel_mm <N>
*              N is the number of rows an columns
*
 Square matrix multiplication

 IT 388 - HW03
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <mpi.h>

double Get_time();
void printMatrix(long *C,int N);

/*  Main starts here */
int main(int argc,char* argv[])
{

    long i,j,k;
    int my_rank, nproc, localN, N;
    MPI_Init(&argc, &argv);
    MPI_Comm comm;
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &nproc);
    MPI_Comm_rank(comm, &my_rank);
    double start, elapsed;

    if(my_rank == 0){
      if (argv[1]==0){
          fprintf(stderr,"\n \t USAGE: <matrix dimension> \n\n");
          exit(1);
      }
      N = atoi(argv[1]); // if read from comand line
  }

    // broadcast N to other cores and set up localN
    MPI_Bcast(&N, 1, MPI_INT, 0, comm);
    localN = (N*N)/nproc;

    //allocates memory for an array of N*N long
    long* A = malloc(N*N*sizeof(long));
    long* B = malloc(N*N*sizeof(long));
    long* C = malloc(N*N*sizeof(long));
    long* local_y = malloc(localN * sizeof(long));
    long* local_x = malloc(localN * sizeof(long));
    long* local_z = malloc(localN * sizeof(long));


    // Initialize matrices.
    if(my_rank == 0){
      for ( i = 0; i < N; ++i) {
          for ( j = 0; j < N; ++j) {
              A[i*N+j] = (i%2) + j%3;
              B[i*N+j] = (i%3) - j%2;
          }
      }
    }

    //scatter A
    MPI_Scatter(A, localN, MPI_LONG, local_y, localN, MPI_LONG, 0, comm);
    //scatter B
    MPI_Scatter(B, localN, MPI_LONG, local_x, localN, MPI_LONG, 0, comm);

    MPI_Barrier(comm);
    start = Get_time();


    // Compute matrix multiplication.
    // C <- C + A x B
    for ( i = 0; i < N; ++i) {
        for ( j = 0; j < N; ++j) {
            local_z[i*N+j]=0;
            for ( k = 0; k < N; ++k) {
                local_z[i*N+j] += local_y[i*N+k] * local_x[k*N+j];
            }
        }
    }

    // gather all local_z from cores and store in C
    MPI_Gather(local_z, localN, MPI_LONG, C, localN, MPI_LONG, 0, comm);

    MPI_Barrier(comm);
    elapsed = Get_time()-start;

    if(my_rank == 0){
      printf("Matrix Dimension: C[%d,%d]\tElapsed time: %f sec\n",N,N, elapsed);
      if (N<=20){
          printMatrix(C,N);
          return 0;
        }
    }
    MPI_Finalize();
} /* end main */


/** Returns the wall clock time  */
double Get_time(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec/1000000.0;
}

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
