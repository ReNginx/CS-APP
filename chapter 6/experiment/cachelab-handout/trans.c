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
#define __tmp 4
#define _tmp 8
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	if(M == 64) {
		int i, j, k, l, sgn = 1;
		for (i = 0; i < N; i += _tmp)
			for (j = 0; j < M; j += __tmp) {
				if (sgn == 1)
					for (k = 0; k < _tmp && i + k < N; ++k) {
						int x[__tmp] = {};
						for (l = 0; l < __tmp && j + l < M; ++l)
							x[l] = A[i+k][j+l];
							
						for (l = 0; l < __tmp && j + l < M; ++l)
							B[j+l][i+k] = x[l];
					}
				else
					for (k = 0; k < _tmp && i + k < N; ++k) {
						int x[__tmp] = {};
						for (l = 0; l < __tmp && j + l < M; ++l)
							x[l] = A[i+(_tmp-k-1)][j+l];
					
						for (l = 0; l < __tmp && j + l < M; ++l)
							B[j+l][i+(_tmp-k-1)] = x[l];
					}
				sgn = -sgn;
			}
	}
	else {
		int i, j, k, l;
		for (j = 0; j < M; j += _tmp)
			for (i = 0; i < N; i += _tmp)
				for (k = 0; k < _tmp && i + k < N; ++k) {
					int x[_tmp] = {};
					for (l = 0; l < _tmp && j + l < M; ++l)
						x[l] = A[i+k][j+l];
				
					for (l = 0; l < _tmp && j + l < M; ++l)
						B[j+l][i+k] = x[l];
				}
	}
}


char transpose_submit1_desc[] = "Transpose submission1";
void transpose_submit1(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, k, l, sgn = 1;
	for (j = 0; j < M; j += __tmp)
		for (i = 0; i < N; i += _tmp)
			for (k = 0; k < _tmp && i + k < N; ++k) {
				int x[__tmp] = {};
				if (sgn == 1) {
					for (l = 0; l < __tmp && j + l < M; ++l)
						x[l] = A[i+k][j+l];
				
					for (l = 0; l < __tmp && j + l < M; ++l)
						B[j+l][i+k] = x[l];
				}else {					
					for (l = 0; l < __tmp && j + l < M; ++l)
						x[l] = A[i+k][j+(__tmp-l-1)];
				
					for (l = 0; l < __tmp && j + l < M; ++l)
						B[j+(__tmp-l-1)][i+k] = x[l];
				}
				sgn = -sgn;
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
    /* registerTransFunction(transpose_submit1, transpose_submit_desc1);  */
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

