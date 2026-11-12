#include "nqueens.h"
#include <omp.h>

#ifndef N_QUEENS
#define N_QUEENS 15
#endif

int main(void)
{
    const int n = N_QUEENS;
    if (n <= 0) {
        fprintf(stderr, "N must be positive\n");
        return 1;
    }

    int* board = (int*)malloc(n * sizeof(int));
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        board[i] = -1;
    }

    omp_set_dynamic(0);
    double start_time = omp_get_wtime();
    long long solutions = solveNQueensUtil(board, 0, n);
    double end_time = omp_get_wtime();
    printf("Solutions: %lld\n", solutions);
    printf("Time taken: %f seconds\n", end_time - start_time);

    free(board);
    return 0;
}
