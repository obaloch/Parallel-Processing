# Parallel-Processing

mpi_sum.c: MPI code to compute the sum of the first N
integer numbers (1 + 2 + 3 + … + N) using multiple processors. The work of computing the sum
should be divided among the processors. The manager processor should print the total sum on
the screen

parallel_mm.c: parallel matrix-matrix multiplication in MPI

omp_mm1.c & omp_mm2.c: parallel matrix-matrix multiplication in openMP. omp_mm1.c is parallelized for the outer loop(i-loop)
and omp_mm2.c is parallelized for the inner loop (j-loop).

cuda_matrixAdd.c:Implement matrix addition using CUDA. Matrix addition is given by ANxM + BNxM=CNxM
where N is the number of rows and M is the number of columns.

cuda_vectorAdd.c: Implement vector addition using CUDA
