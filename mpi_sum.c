/* File: mpi_sum.c
 IT 388 - Spring 2022
 Homework 2
 Your Name: Omar Baloch
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> // <mpi.h> for mpi functions

void printArray(int [],int size);

int main (int argc, char* argv[]) {

  int N, my_rank, nproc, work, offset, dest, source;
  int sum;
  // int *array;
  MPI_Init(&argc,&argv);
  MPI_Comm comm; MPI_Status status; comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &nproc);
  MPI_Comm_rank(comm, &my_rank);
  int tag = 15;

  if(my_rank == 0){
    // 1. Accepts the value N on the command line argument
    printf("Enter integer N: ");
    // 2. Gives error if user does not provide it
    if((scanf("%d", &N)) == 1){
      printf("You entered an integer.");
    } else {
      printf("No input. Terminate.\n");
      exit(1);
    }

    // Define local amount of items each processor will work on and their offset
    work = N / nproc;
    offset = my_rank * work;
    sum = 0;

    // 4. manager sends subbarrays AND value of 'N' to each core
    for(dest = 1; dest < nproc; dest++){
      offset = dest * work + 1;
      MPI_Send(&N, 1, MPI_INT, dest, 2, comm);
      MPI_Send(&offset, 1, MPI_INT, dest, tag, comm);
    }

    //manager does its part
    for(int i = 1; i < work + 1; i++){
      sum += i;
    }

    int total = sum;
    printf("Rank = %d, Sum = %d\n", my_rank, sum);


    /* manager recvs sums from child processes that they completed according
    to their work and also add that to the total
    */
    for(source = 1; source < nproc; source++){
      MPI_Recv(&sum, 1, MPI_INT, source, 3, comm, &status);
      total += sum;
    }

    // print the total
    printf("Total: %d\n", total);

  } else {

    //recv value of N from manager
    MPI_Recv(&N, 1, MPI_INT, 0, 2, comm, &status);
    work = N / nproc;
    // recive offset from the manager core
    MPI_Recv(&offset, 1, MPI_INT, 0, tag, comm, &status);
    // get sum from offset upto whatever value work is
    sum = 0;
    for(int i = offset; i < offset + work; i++){
      sum += i;
    }
    //print rank and individual sum
    printf("Rank = %d, Sum = %d\n", my_rank, sum);
    //send manager core the individual sums
    MPI_Send(&sum, 1, MPI_INT, 0, 3, comm);

  }
  MPI_Finalize();
}
