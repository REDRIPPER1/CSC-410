  # CSC 410 Assignment 1

This assignment implements and measures five algorithms in C:

- Array summation in `arraysum.c`
- Square matrix multiplication in `matrixMul/`
- The N-Queens backtracking solver in `Nqueens/`
- Bubble sort and merge sort in `sorts/`
- Trapezoidal numerical integration in `numIntegrate.c`

## Requirements

- GCC or another C11-compatible compiler
- GNU Make
- `/usr/bin/time` for the timing target on Linux or WSL

## Build

From this directory, build all programs with:

```bash
make
```

Executables are placed in the `build/` directory. To remove generated files:

```bash
make clean
```

## Run

Run every program sequentially:

```bash
make run
```

Run an individual program:

```bash
./build/arraysum
./build/numIntegrate
./build/matrixMul
./build/nqueens
./build/sorts
```

The default workloads are configured in the source files. They include a 1,000
by 1,000 matrix multiplication and 100,000 integration intervals.

## Timing

Build and time all five programs while suppressing their normal output:

```bash
make time
```

The timing output reports elapsed time in seconds and peak memory usage in KB.
The N-Queens and sorting defaults are intentionally small enough to run quickly;
increase their input constants in `Nqueens/mainQ.c` and `sorts/sorts.h` when
testing larger workloads.
