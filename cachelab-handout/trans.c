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
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int val0,val1,val2,val3,val4,val5,val6,val7;
	for(int ii=0;ii<N;ii+=8)
	{
		for(int jj=0;jj<M;jj+=8)
		{
			for(int i=ii;i<ii+8;++i)
			{
				int j = jj;
				val0 = A[i][j];
				val1 = A[i][j+1];
				val2 = A[i][j+2];
				val3 = A[i][j+3];
				val4 = A[i][j+4];
				val5 = A[i][j+5];
				val6 = A[i][j+6];
				val7 = A[i][j+7];
				B[j][i] = val0;
				B[j+1][i] = val1;
				B[j+2][i] = val2;
				B[j+3][i] = val3;
				B[j+4][i] = val4;
				B[j+5][i] = val5;
				B[j+6][i] = val6;
				B[j+7][i] = val7;
			}
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
char trans_desc[] = "blocksize 4 by 4";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int val0, val1, val2, val3;
	for(int ii=0; ii<N; ii+=4)
	{
		for(int jj=0;jj<M; jj+=4)
		{
			for(int i=ii;i<ii+4;++i)
			{
				int j = jj;
				val0 = A[i][j];
				val1 = A[i][j+1];
				val2 = A[i][j+2];
				val3 = A[i][j+3];
				B[j][i] = val0;
				B[j+1][i] = val1;
				B[j+2][i] = val2;
				B[j+3][i] = val3;
			}
		}
	}

}

char trans_desc2[] = "blocksize 16 by 16";
void trans2(int M, int N, int A[N][M], int B[M][N])
{
	int ii, jj, val0, val1, val2, val3, val4, val5,val6, val7;
	for(ii=0; ii+16<N; ii+=16)
	{
		for(jj=0;jj+16<N; jj+=16)
		{
			for(int i=ii;i<ii+16;++i)
			{
				int j = jj;
				val0 = A[i][j];
				val1 = A[i][j+1];
				val2 = A[i][j+2];
				val3 = A[i][j+3];
				val4 = A[i][j+4];
				val5 = A[i][j+5];
				val6 = A[i][j+6];
				val7 = A[i][j+7];
				B[j][i] = val0;
				B[j+1][i] = val1;
				B[j+2][i] = val2;
				B[j+3][i] = val3;
				B[j+4][i] = val4;
				B[j+5][i] = val5;
				B[j+6][i] = val6;
				B[j+7][i] = val7;
				val0 = A[i][j+8];
				val1 = A[i][j+9];
				val2 = A[i][j+10];
				val3 = A[i][j+11];
				val4 = A[i][j+12];
				val5 = A[i][j+13];
				val6 = A[i][j+14];
				val7 = A[i][j+15];
				B[j+8][i] = val0;
				B[j+9][i] = val1;
				B[j+10][i] = val2;
				B[j+11][i] = val3;
				B[j+12][i] = val4;
				B[j+13][i] = val5;
				B[j+14][i] = val6;
				B[j+15][i] = val7;

			}
		}

	}
	for(;ii<N;++ii)
	{
		for(int j=0;j<M;++j)
		{
			B[j][ii] = A[ii][j];
		}
	}
	for(;jj<M;++jj)
	{
		for(int i=0;i<N;++i)
		{
			B[jj][i] = A[i][jj];
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
    registerTransFunction(trans2, trans_desc2);	

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

