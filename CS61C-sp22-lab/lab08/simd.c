#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        __m128i sum = _mm_setzero_si128();
        unsigned int i;
        for(i = 0; i < NUM_ELEMS - 4; i += 4) {
            __m128i date = _mm_loadu_si128((__m128i *)(vals + i));
            __m128i mask = _mm_cmpgt_epi32(date, _127);
            __m128i selected = _mm_and_si128(date, mask);
            sum = _mm_add_epi32(sum, selected);
        }

        int temp_arr[4];
        _mm_storeu_si128((__m128i *)temp_arr, sum);
        for (int k = 0; k < 4; k++) {
            result += temp_arr[k];
        }

        for(; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
        /* Hint: you'll need a tail case. */
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        /* Copy your sum_simd() implementation here, and unroll it */
        __m128i sum0 = _mm_setzero_si128();
        __m128i sum1 = _mm_setzero_si128();
        __m128i sum2 = _mm_setzero_si128();
        __m128i sum3 = _mm_setzero_si128();
        unsigned int i;
        for(i = 0; i < NUM_ELEMS - 16; i += 16) {
            __m128i date0 = _mm_loadu_si128((__m128i *)(vals + i));
            __m128i date1 = _mm_loadu_si128((__m128i *)(vals + i + 4));
            __m128i date2 = _mm_loadu_si128((__m128i *)(vals + i + 8));
            __m128i date3 = _mm_loadu_si128((__m128i *)(vals + i + 12));

            __m128i mask0 = _mm_cmpgt_epi32(date0, _127);
            __m128i mask1 = _mm_cmpgt_epi32(date1, _127);
            __m128i mask2 = _mm_cmpgt_epi32(date2, _127);
            __m128i mask3 = _mm_cmpgt_epi32(date3, _127);

            __m128i selected0 = _mm_and_si128(date0, mask0);
            __m128i selected1 = _mm_and_si128(date1, mask1);
            __m128i selected2 = _mm_and_si128(date2, mask2);
            __m128i selected3 = _mm_and_si128(date3, mask3);

            sum0 = _mm_add_epi32(sum0, selected0);
            sum1 = _mm_add_epi32(sum1, selected1);
            sum2 = _mm_add_epi32(sum2, selected2);
            sum3 = _mm_add_epi32(sum3, selected3);
        }

        sum0 = _mm_add_epi32(sum0, sum1);
        sum0 = _mm_add_epi32(sum0, sum2);
        sum0 = _mm_add_epi32(sum0, sum3);

        int temp_arr[4];
        _mm_storeu_si128((__m128i *)temp_arr, sum0);
        for (int k = 0; k < 4; k++) {
            result += temp_arr[k];
        }

        for(; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
        /* Hint: you'll need 1 or maybe 2 tail cases here. */
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
