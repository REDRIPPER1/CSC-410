#include <stdio.h>
#include <omp.h>

#ifndef INTERVALS
#define INTERVALS 1000000000LL
#endif

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule(void)
{
    const double lowerBound = 0.0;
    const double upperBound = 1.0;
    const double intervalWidth = (upperBound - lowerBound) / (double)INTERVALS;
    double sum = (f(lowerBound) + f(upperBound)) / 2.0;

    #pragma omp parallel for reduction(+:sum)
    for (long long i = 1; i < INTERVALS; ++i) {
        sum += f(lowerBound + i * intervalWidth);
    }

    return sum * intervalWidth;
}

int main(void) {
    if (INTERVALS <= 0) {
        fprintf(stderr, "Interval count must be positive\n");
        return 1;
    }

    omp_set_dynamic(0);
    double start_time = omp_get_wtime();
    double pi = trapezoidalRule();
    double end_time = omp_get_wtime();
    printf("Estimated value of π: %f\n", pi);
    printf("Time taken: %f seconds\n", end_time - start_time);
    return 0;
}
