#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool isSafe(int board[], int row, int col, int n) 
{
    (void)n;
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

void solveNQueensUtil(int board[], int col, int n) 
{
    if (col == n) {
        for (int row = 0; row < n; ++row) {
            printf("%d ", board[row]);
        }
        printf("\n");
        return;
    }

    for (int row = 0; row < n; ++row) {
        if (isSafe(board, row, col, n)) {
            board[col] = row;
            solveNQueensUtil(board, col + 1, n);
            board[col] = -1;
        }
    }
}