#include "transpose.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    // YOUR CODE HERE
    for (int row = 0; row < n; row += blocksize) {
        for (int col = 0; col < n; col += blocksize) {

            int row_end = MIN(row + blocksize, n);
            int col_end = MIN(col + blocksize, n);
            
            for (int i = row; i < row_end; i++) {
                for (int j = col; j < col_end; j++) {
                    dst[j * n + i] = src[i * n + j];
                }
            }
        }
    }
}
