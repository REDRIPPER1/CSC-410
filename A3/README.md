# CSC 410 Assignment 3: OpenMP

This directory contains OpenMP implementations of the Assignment 3 programs:

- Array summation in `arraysum.c`
- Square matrix multiplication in `matrixMul/`
- Trapezoidal numerical integration in `numIntegrate.c`
- The N-Queens backtracking solver in `Nqueens/`

## Requirements

- GCC with OpenMP support

## Build and run

Compile each program from this directory:

```bash
gcc -O2 -fopenmp arraysum.c -o arraysum
gcc -O2 -fopenmp matrixMul/mainM.c -o matrixMul/matrixMul
gcc -O2 -fopenmp numIntegrate.c -o numIntegrate
gcc -O2 -fopenmp Nqueens/mainQ.c -o Nqueens/nqueens
```

The default workloads match the assignment: 1,000,000,000 array elements,
1,000 x 1,000 matrices, 1,000,000,000 integration intervals, and N = 15.
Set `OMP_NUM_THREADS` to select the OpenMP thread count:

```bash
OMP_NUM_THREADS=1 ./arraysum
OMP_NUM_THREADS=2 ./arraysum
OMP_NUM_THREADS=5 ./arraysum
OMP_NUM_THREADS=7 ./arraysum
OMP_NUM_THREADS=9 ./arraysum
```

Repeat those thread counts for the other three executables. Use a smaller
compile-time workload while checking behavior, for example:

```bash
gcc -O2 -fopenmp -DARRAY_SIZE=10000 arraysum.c -o arraysum-test
gcc -O2 -fopenmp -DINTERVALS=10000 numIntegrate.c -o numIntegrate-test
gcc -O2 -fopenmp -DN=32 matrixMul/mainM.c -o matrixMul-test
gcc -O2 -fopenmp -DN_QUEENS=8 Nqueens/mainQ.c -o nqueens-test
```

Each program reports its result and elapsed time measured with `omp_get_wtime()`.

