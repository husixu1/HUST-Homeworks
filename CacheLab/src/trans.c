/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    int a, b, c, d;
    int reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7;
    if (M == 32 && N == 32) {
        for (a = 0; a < N; a += 8) {
            for (b = 0; b < M; b += 8) {
                for (c = a; c < a + 8; ++c) {
                    reg0 = A[c][b + 0];
                    reg1 = A[c][b + 1];
                    reg2 = A[c][b + 2];
                    reg3 = A[c][b + 3];
                    reg4 = A[c][b + 4];
                    reg5 = A[c][b + 5];
                    reg6 = A[c][b + 6];
                    reg7 = A[c][b + 7];
                    B[b + 0][c] = reg0;
                    B[b + 1][c] = reg1;
                    B[b + 2][c] = reg2;
                    B[b + 3][c] = reg3;
                    B[b + 4][c] = reg4;
                    B[b + 5][c] = reg5;
                    B[b + 6][c] = reg6;
                    B[b + 7][c] = reg7;
                }
            }
        }
    } else if (M == 64 && N == 64) {
        for (a = 0; a < N; a += 8) {
            for (b = 0; b < M; b += 8) {
                for (c = a; c < a + 4; ++c) {
                    reg0 = A[c][b + 0];
                    reg1 = A[c][b + 1];
                    reg2 = A[c][b + 2];
                    reg3 = A[c][b + 3];
                    reg4 = A[c][b + 4];
                    reg5 = A[c][b + 5];
                    reg6 = A[c][b + 6];
                    reg7 = A[c][b + 7];
                    B[b + 0][c] = reg0;
                    B[b + 0][c + 4] = reg4;
                    B[b + 1][c] = reg1;
                    B[b + 1][c + 4] = reg5;
                    B[b + 2][c] = reg2;
                    B[b + 2][c + 4] = reg6;
                    B[b + 3][c] = reg3;
                    B[b + 3][c + 4] = reg7;
                }
                for (c = 0; c < 4; ++c) {
                    reg0 = A[a + 4][b + c];
                    reg1 = A[a + 5][b + c];
                    reg2 = A[a + 6][b + c];
                    reg3 = A[a + 7][b + c];
                    reg4 = B[b + c][a + 4];
                    reg5 = B[b + c][a + 5];
                    reg6 = B[b + c][a + 6];
                    reg7 = B[b + c][a + 7];
                    B[b + c][a + 4] = reg0;
                    B[b + c][a + 5] = reg1;
                    B[b + c][a + 6] = reg2;
                    B[b + c][a + 7] = reg3;
                    reg0 = A[a + 4][b + c + 4];
                    reg1 = A[a + 5][b + c + 4];
                    reg2 = A[a + 6][b + c + 4];
                    reg3 = A[a + 7][b + c + 4];
                    B[b + c + 4][a + 0] = reg4;
                    B[b + c + 4][a + 4] = reg0;
                    B[b + c + 4][a + 1] = reg5;
                    B[b + c + 4][a + 5] = reg1;
                    B[b + c + 4][a + 2] = reg6;
                    B[b + c + 4][a + 6] = reg2;
                    B[b + c + 4][a + 3] = reg7;
                    B[b + c + 4][a + 7] = reg3;
                }
            }
        }
    } else if (M == 61 && N == 67) {
        for (a = 0; a < N; a += 23)
            for (b = 0;  b < M; b += 23)
                for (c = a; c < a + 23 && c < N; ++c)
                    for (d = b; d < b + 23 && d < M; ++d)
                        B[d][c] = A[c][d];
    }

}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
