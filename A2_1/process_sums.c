// convert sequential sums to parallel

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#ifndef process
#define process 4
#endif

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int *arr = malloc(N * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    int pipes[process][2];
    pid_t children[process];

    for (int i = 0; i < process; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            for (int j = 0; j < i; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            free(arr);
            return 1;
        }
    }

    for (int i = 0; i < process; i++) {
        children[i] = fork();

        if (children[i] == -1) {
            perror("fork");
            for (int j = 0; j < process; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            for (int j = 0; j < i; j++) {
                waitpid(children[j], NULL, 0);
            }
            free(arr);
            return 1;
        }

        if (children[i] == 0) {
            for (int j = 0; j < process; j++) {
                close(pipes[j][0]);
                if (j != i) {
                    close(pipes[j][1]);
                }
            }

            int start = i * N / process;
            int end = (i + 1) * N / process;
            long long partial_sum = 0;

            for (int j = start; j < end; j++) {
                partial_sum += arr[j];
            }

            if (write(pipes[i][1], &partial_sum, sizeof(partial_sum)) !=
                (ssize_t)sizeof(partial_sum)) {
                perror("write");
                close(pipes[i][1]);
                free(arr);
                return 1;
            }
            close(pipes[i][1]);
            free(arr);
            return 0;
        }
    }

    for (int i = 0; i < process; i++) {
        close(pipes[i][1]);
    }

    for (int i = 0; i < process; i++) {
        if (waitpid(children[i], NULL, 0) == -1) {
            perror("waitpid");
            for (int j = i; j < process; j++) {
                close(pipes[j][0]);
            }
            free(arr);
            return 1;
        }
    }

    long long total = 0;
    for (int i = 0; i < process; i++) {
        long long partial_sum;
        if (read(pipes[i][0], &partial_sum, sizeof(partial_sum)) !=
            (ssize_t)sizeof(partial_sum)) {
            perror("read");
            close(pipes[i][0]);
            free(arr);
            return 1;
        }
        close(pipes[i][0]);
        total += partial_sum;
    }

    printf("Total sum = %lld\n", total);
    free(arr);
    
    return 0;
}
