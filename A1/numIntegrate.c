#include <stdio.h>

#define N 100000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule() 
{
    const double lowerBound = 0.0;
    const double upperBound = 1.0;
    const double intervalWidth = (upperBound - lowerBound) / N;
    double sum = (f(lowerBound) + f(upperBound)) / 2.0;

    for (int i = 1; i < N; ++i) {
        sum += f(lowerBound + i * intervalWidth);
    }

    return sum * intervalWidth;
}

int main() {
    double pi = trapezoidalRule();
    printf("Estimated value of π: %f\n", pi);
    return 0;
}
