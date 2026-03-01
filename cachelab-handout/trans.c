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

void transpose_32(int A[32][32],int B[32][32]);
void transpose_61(int A[67][61],int B[61][67]);
void transpose_64(int A[64][64], int B[64][64]);
/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";


void transpose_submit (int M, int N, int A[N][M] , int B[M][N]){


	if(N==32 && M==32)transpose_32( A,B);
    if (M ==61 && N==67) transpose_61(A,B);



}

void transpose_32( int A[32][32], int B[32][32])
{

    // given that the cache size is 32 bytes we want to reduce the amount of conflict misses
    // by bringing in less distinct cache sets into the cache
    int i,j,i1,j1,tmp,diag;

    for (i = 0; i < 32; i += 8) {
        for (j = 0; j < 32; j += 8) {
            for (i1 = i; i1 < i + 8; i1++) {
                for ( j1 = j; j1 < j + 8; j1++) {

			
               
 			if( i1!=j1){


				
				
			B[j1][i1] = A[i1][j1];


			}
			else{


				tmp = A[i1][j1];
				diag = i1;



		}
            
		}
		if(i==j)B[diag][diag] = tmp;
        }
    }




    }
    }


void transpose_61( int A[67][61], int B[61][67])
{

    // given that the cache 
    int i,j,i1,j1;

    for (i = 0; i < 67; i += 16) {
        for (j = 0; j < 61; j += 16) {
            for (i1 = i; i1 < i + 16&& i1 <67; i1++) {
                for ( j1 = j; j1 < j + 16 && j1<61; j1++) {

			

			B[j1][i1] = A[i1][j1];


			

			
    }




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
    //registerTransFunction(trans, trans_desc); 

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

