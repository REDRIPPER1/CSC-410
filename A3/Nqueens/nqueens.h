#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

long long solveNQueensUtil(int board[], int col, int n)
{
    if (col == n) {
        return 1;
    }

    if (col == 0) {
        long long solutions = 0;
        #pragma omp parallel for reduction(+:solutions) schedule(dynamic)
        for (int row = 0; row < n; ++row) {
            int branchBoard[n];
            for (int i = 0; i < n; ++i) {
                branchBoard[i] = board[i];
            }
            branchBoard[0] = row;
            solutions += solveNQueensUtil(branchBoard, 1, n);
        }
        return solutions;
    }

    long long solutions = 0;
    for (int row = 0; row < n; ++row) {
        if (isSafe(board, row, col, n)) {
            board[col] = row;
            solutions += solveNQueensUtil(board, col + 1, n);
            board[col] = -1;
        }
    }
    return solutions;
}