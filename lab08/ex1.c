#include "ex1.h"

void v_add_naive(double *x, double *y, double *z) {
#pragma omp parallel
    {
        for (int i = 0; i < ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double *x, double *y, double *z) {
    // TODO: Implement this function
    // Do NOT use the `for` directive here!
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        for (int i = thread_id; i < ARRAY_SIZE; i += num_threads) {
            z[i] = x[i] + y[i];
        }
    }
}

// Chunks Method
void v_add_optimized_chunks(double *x, double *y, double *z) {
    // TODO: Implement this function
    // Do NOT use the `for` directive here!
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // 计算每个线程处理的元素数量
        int chunk_size = ARRAY_SIZE / num_threads;
        int remainder = ARRAY_SIZE % num_threads;

        // 计算当前线程的起始和结束索引
        int start, end;

        if (thread_id < remainder) {
            // 前 remainder 个线程多处理一个元素
            start = thread_id * (chunk_size + 1);
            end = start + chunk_size;  // 因为多一个，所以是 chunk_size
        } else {
            // 后面的线程
            start = thread_id * chunk_size + remainder;
            end = start + chunk_size - 1;
        }

        // 处理当前线程的连续块
        for (int i = start; i <= end; i++) {
            z[i] = x[i] + y[i];
        }
    }
}
