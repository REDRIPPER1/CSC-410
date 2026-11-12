#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE 1000000000LL
#endif

unsigned long long sumArray(const int arr[], long long size)
{
    unsigned long long total = 0;

    #pragma omp parallel for reduction(+:total)
    for (long long i = 0; i < size; ++i) {
        total += (unsigned int)arr[i];
    }

    return total;
}

int main(void)
{
    if (ARRAY_SIZE <= 0 || (unsigned long long)ARRAY_SIZE > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Array size is invalid or too large\n");
        return 1;
    }

    int *arr = malloc((size_t)ARRAY_SIZE * sizeof(*arr));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed for %lld integers\n",
                (long long)ARRAY_SIZE);
        return 1;
    }

    omp_set_dynamic(0);
    #pragma omp parallel for
    for (long long i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = i + 1; 
    }

    double start_time = omp_get_wtime();
    unsigned long long totalSum = sumArray(arr, ARRAY_SIZE);
    double end_time = omp_get_wtime();
    printf("Total Sum: %llu\n", totalSum);
    printf("Time taken: %f seconds\n", end_time - start_time);

    free(arr);
    return 0;
}
