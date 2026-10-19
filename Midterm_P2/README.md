# CSC 410 Midterm Part 2

## Files

- `sobel.c` — sequential, pthread, and OpenMP Sobel implementations. The
default build uses pthreads.
- `rw.c` — custom FIFO reader-writer lock demonstration.
- `pdf.pdf` — benchmark results.
- `stb_image.h` and `stb_image_write.h` — image helpers.
- `NGGYU.jpg` and `NGGYU2.jpg` — input images.

## Build and run

### Pthreads Sobel

```sh
gcc sobel.c -lm -o sobel
./sobel NGGYU.jpg 4
./sobel NGGYU2.jpg 8
```

The pthread version divides the interior image rows evenly. Any remaining
rows are assigned one each to the first workers. Workers write separate
output rows, so no lock is needed for image processing. Output files include
the thread count, such as `output_sobel_pthread_t4.bmp`.

### OpenMP Sobel

```sh
gcc -O2 -DSOBEL_OPENMP -fopenmp sobel.c -lm -o sobel_openmp
./sobel_openmp NGGYU2.jpg 8
```

The OpenMP version uses `collapse(2)` and static scheduling to distribute
the nested row and column loops. Each loop iteration writes to a different
pixel. Output files include the thread count, such as
`output_sobel_openmp_t8.bmp`. OpenMP needs `-fopenmp` at compile and link time.

### Sequential baseline

```sh
gcc -O2 -DSOBEL_SEQUENTIAL sobel.c -lm -o sobel_seq
./sobel_seq NGGYU.jpg
```

### Reader-writer lock

```sh
gcc -O2 rw.c -pthread -o rw
./rw
```

## Implementation notes

All Sobel modes apply the same 3-by-3 X/Y kernels to RGB channels, multiply
the magnitude by 2, clamp it to 255, and leave the outer border black.
Parallel outputs include the requested thread count in the filename, for
example `output_sobel_pthread_t4.bmp` and `output_sobel_openmp_t4.bmp`.

The sequential and pthread implementations use `CLOCK_MONOTONIC`; OpenMP
uses `omp_get_wtime`. Image loading and saving are excluded from the
measurements.

The reader-writer lock uses FIFO tickets so new readers or writers cannot
bypass threads already waiting. Consecutive readers may hold the lock
together. Condition-variable waits recheck their conditions in loops.

The benchmark report gives three-run median timings for both images with 1,
2, 3, 4, and 8 threads. Results depend on the machine and system load;
additional threads may not improve performance, especially for the smaller
image.
