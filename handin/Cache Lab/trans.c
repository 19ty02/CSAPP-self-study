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

void trans4_1(int A[32][32], int B[32][32], int xa, int ya, int xb, int yb)
{
    int t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;
    t1 = A[xa + 1][ya]; t2 = A[xa + 1][ya + 1]; t3 = A[xa + 1][ya + 2]; t4 = A[xa + 1][ya + 3];
    t5 = A[xa + 2][ya]; t6 = A[xa + 2][ya + 1]; t7 = A[xa + 2][ya + 2]; t8 = A[xa + 2][ya + 3];
    t9 = A[xa + 3][ya]; t10 = A[xa + 3][ya + 1]; t11 = A[xa + 3][ya + 2]; t12 = A[xa + 3][ya + 3];

    B[xb + 1][yb] = A[xa][ya + 1];
    B[xb + 2][yb] = A[xa][ya + 2];
    B[xb + 3][yb] = A[xa][ya + 3];
    B[xb][yb] = A[xa][ya];      // 这步一定要放在最后

    B[xb][yb + 1] = t1; B[xb + 1][yb + 1] = t2; B[xb + 2][yb + 1] = t3; B[xb + 3][yb + 1] = t4;
    B[xb][yb + 2] = t5; B[xb + 1][yb + 2] = t6; B[xb + 2][yb + 2] = t7; B[xb + 3][yb + 2] = t8;
    B[xb][yb + 3] = t9; B[xb + 1][yb + 3] = t10; B[xb + 2][yb + 3] = t11; B[xb + 3][yb + 3] = t12;
}

void work1(int A[32][32], int B[32][32])
{
    for (int ii=0; ii<4; ii++) {
        for (int jj=0; jj<4; jj++) {
            if (ii == jj) {
                trans4_1(A, B, ii*8, jj*8, ii*8, jj*8);
                trans4_1(A, B, ii*8 + 4, jj*8, ii*8, jj*8 + 4);
                trans4_1(A, B, ii*8 + 4, jj*8 + 4, ii*8 + 4, jj*8 + 4);
                trans4_1(A, B, ii*8, jj*8 + 4, ii*8 + 4, jj*8);
            } else {
                for (int i=0; i<8; i++) {
                    for (int j=0; j<8; j++) {
                        B[ii*8+i][jj*8+j] = A[jj*8+j][ii*8+i];
                    }
                }
            }
        }
    }
}

// 8*4
void trans4_2(int A[64][64], int B[64][64], int xa, int ya, int xb, int yb)
{
    int t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12;
    t1 = A[xa + 1][ya]; t2 = A[xa + 1][ya + 1]; t3 = A[xa + 1][ya + 2]; t4 = A[xa + 1][ya + 3];
    t5 = A[xa + 2][ya]; t6 = A[xa + 2][ya + 1]; t7 = A[xa + 2][ya + 2]; t8 = A[xa + 2][ya + 3];
    t9 = A[xa + 3][ya]; t10 = A[xa + 3][ya + 1]; t11 = A[xa + 3][ya + 2]; t12 = A[xa + 3][ya + 3];

    B[xb + 1][yb] = A[xa][ya + 1];
    B[xb + 2][yb] = A[xa][ya + 2];
    B[xb + 3][yb] = A[xa][ya + 3];
    B[xb][yb] = A[xa][ya]; 

    B[xb][yb + 1] = t1; B[xb + 1][yb + 1] = t2; B[xb + 2][yb + 1] = t3; B[xb + 3][yb + 1] = t4;
    B[xb][yb + 2] = t5; B[xb + 1][yb + 2] = t6; B[xb + 2][yb + 2] = t7; B[xb + 3][yb + 2] = t8;
    B[xb][yb + 3] = t9; B[xb + 1][yb + 3] = t10; B[xb + 2][yb + 3] = t11; B[xb + 3][yb + 3] = t12;
}

// 32变18 （若按上面的写法则是32变20），其实可以做到32变17但没必要，已经能拿满分
void trans8_2(int A[64][64], int B[64][64], int xa, int ya, int xb ,int yb) {
    int t1, t2, t3, t4, t5, t6, t7, t8;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            B[xb + j][yb + i] = A[xa + i][ya + j];
        }
    }
    t1 = A[xa + 2][ya + 4]; t2 = A[xa + 2][ya + 5]; t3 = A[xa + 2][ya + 6]; t4 = A[xa + 2][ya + 7];
    t5 = A[xa + 3][ya + 4]; t6 = A[xa + 3][ya + 5]; t7 = A[xa + 3][ya + 6]; t8 = A[xa + 3][ya + 7];
    for (int i=4; i<8; i++) {
        for (int j=0; j<4; j++) {
            B[xb + j][yb + i] = A[xa + i][ya + j];
        }
    }
    for (int i=4; i<8; i++) {
        for (int j=4; j<8; j++) {
            B[xb + j][yb + i] = A[xa + i][ya + j];
        }
    }

    for (int i=0; i<2; i++) {
        for (int j=4; j<8; j++) {
            B[xb + j][yb + i] = A[xa + i][ya + j];
        }
    }
    B[xb + 4][yb + 2] = t1; B[xb + 5][yb + 2] = t2; B[xb + 6][yb + 2] = t3; B[xb + 7][yb + 2] = t4;
    B[xb + 4][yb + 3] = t5; B[xb + 5][yb + 3] = t6; B[xb + 6][yb + 3] = t7; B[xb + 7][yb + 3] = t8;
}

void work2(int A[64][64], int B[64][64])
{
    // for (int ii=0; ii<8; ii++) {
    //     for (int jj=0; jj<16; jj++) {
    //         for (int i=0; i<8; i++) {
    //             for (int j=0; j<4; j++) {
    //                 B[jj * 4 + j][ii * 8 + i] = A[ii * 8 + i][jj * 4 + j];
    //             }
    //         }
    //     }
    // }
    for (int ii=0; ii<8; ii++) {
        for (int jj=0; jj<8; jj++) {
            if (ii != jj) {
                trans8_2(A, B, ii*8, jj*8, jj*8, ii*8);
            } else {
                trans4_2(A, B, ii*8 + 4, jj*8 + 4, jj*8 + 4, ii*8 + 4);
                trans4_2(A, B, ii*8 + 4, jj*8, jj*8, ii*8 + 4);
                trans4_2(A, B, ii*8, jj*8, jj*8, ii*8);
                trans4_2(A, B, ii*8, jj*8 + 4, jj*8 + 4, ii*8);
            }
        }
    }
}

void work3(int A[67][61], int B[61][67], int sx, int sy) {
    for (int ii=0; ii*sx < 67; ii++) {
        for (int jj=0; jj*sy < 61; jj++) {
            for (int i = ii * sx; i < (ii+1) * sx && i < 67; i++) {
                for (int j = jj * sy; j < (jj+1) * sy && j < 61; j++) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }
}

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32 && N == 32)
    {
        work1(A, B);
        return;
    }

    if (M == 64 && N == 64)
    {
        work2(A, B);
        return;
    }

    if (M == 61 && N == 67)
    {
        work3(A, B, 8, 16);
        return;
    }
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
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

