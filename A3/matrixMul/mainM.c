#include "matrixMul.h"
#include <omp.h>

int main(void)
{
    if (N <= 0 || (size_t)N > SIZE_MAX / (size_t)N / sizeof(int) ||
        (size_t)N > SIZE_MAX / sizeof(int *)) {
        fprintf(stderr, "Matrix size is invalid or too large\n");
        return 1;
    }

    int** A = calloc((size_t)N, sizeof(*A));
    int** B = calloc((size_t)N, sizeof(*B));
    int** C = calloc((size_t)N, sizeof(*C));
    
    if (A == NULL || B == NULL || C == NULL) {
        fprintf(stderr, "Memory allocation failed for matrix rows\n");
        free(A);
        free(B);
        free(C);
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        A[i] = malloc((size_t)N * sizeof(*A[i]));
        B[i] = malloc((size_t)N * sizeof(*B[i]));
        C[i] = malloc((size_t)N * sizeof(*C[i]));
        if (A[i] == NULL || B[i] == NULL || C[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for matrix data\n");
            for (int j = 0; j <= i; ++j) {
                free(A[j]);
                free(B[j]);
                free(C[j]);
            }
            free(A);
            free(B);
            free(C);
            return 1;
        }
    }

    printf("Matrices allocated successfully.\n");

    // Initialize matrices A and B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    printf("Matrices initialized successfully.\n");

    omp_set_dynamic(0);
    double start_time = omp_get_wtime();
    matrixMultiply(A, B, C, N);
    double end_time = omp_get_wtime();

    printf("Matrix multiplication complete!\n");
    printf("C[0][0]: %d\n", C[0][0]);
    printf("Time taken: %f seconds\n", end_time - start_time);

    // Optionally display the resulting matrix C
    // printf("Resulting Matrix C:\n");
    // displayMatrix(C, N);

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
