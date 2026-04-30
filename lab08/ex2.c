#include "ex2.h"

double dotp_naive(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    for (int i = 0; i < arr_size; i++)
        global_sum += x[i] * y[i];
    return global_sum;
}

// Critical Keyword
double dotp_critical(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // TODO: Implement this function
    // Use the critical keyword here!
#pragma omp parallel
    {
        // 每个线程计算局部和
        double local_sum = 0.0;
#pragma omp for
        for (int i = 0; i < arr_size; i++) {
            local_sum += x[i] * y[i];
        }

        // 将局部和加到全局和（需要critical保护）
#pragma omp critical
        {
            global_sum += local_sum;
        }
    }

    return global_sum;
}

// Reduction Keyword
double dotp_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // TODO: Implement this function
    // Use the reduction keyword here!
#pragma omp parallel for reduction(+:global_sum)
    for (int i = 0; i < arr_size; i++) {
        global_sum += x[i] * y[i];
    }
    return global_sum;
}

// Manual Reduction
double dotp_manual_reduction(double *x, double *y, int arr_size) {
    double global_sum = 0.0;
    // TODO: Implement this function
    // Do NOT use the `reduction` directive here!
    // 获取最大线程数
    int max_threads = omp_get_max_threads();

    // 为每个线程创建局部和数组
    double* local_sums = (double*)calloc(max_threads, sizeof(double));

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // 计算块大小
        int chunk_size = arr_size / num_threads;
        int remainder = arr_size % num_threads;

        // 计算每个线程的起始和结束索引
        int start = thread_id * chunk_size + (thread_id < remainder ? thread_id : remainder);
        int end = start + chunk_size + (thread_id < remainder ? 1 : 0);

        // 计算局部和
        double local_sum = 0.0;
        for (int i = start; i < end; i++) {
            local_sum += x[i] * y[i];
        }

        // 存储到数组
        local_sums[thread_id] = local_sum;
    }

    // 串行求和
    for (int i = 0; i < max_threads; i++) {
        global_sum += local_sums[i];
    }

    free(local_sums);
    return global_sum;
}
