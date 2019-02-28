#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parallel_radix_sort.h"

void map(int* in_arr, int* out_arr, int arr_len, int (*predicate)(int)) {
    #pragma omp parallel
    #pragma omp for
    for (int i = 0; i < arr_len; i++) {
        out_arr[i] = (*predicate)(in_arr[i]);
    }
}

void prescan(int* in_arr, int* out_arr, int arr_len, int identity, int (*operator)(int, int)) {
    memcpy(out_arr, in_arr, arr_len * sizeof(int));
    int steps_num = (int) ceil(log(arr_len) / log(2));

    // Up sweep
    for (int i = 0; i < steps_num; i++) {
        int offset_prev = 1 << i;
        int offset = 1 << (i + 1);

        #pragma omp parallel
        #pragma omp for
        for (int j = 0; j < arr_len; j += offset) {
            int right = arr_len - j - 1;
            int left = arr_len - j - offset_prev - 1;

            if (left >= 0) {
                out_arr[right] = (*operator)(out_arr[left], out_arr[right]);
            }
        }
    }

    // Down sweep
    out_arr[arr_len - 1] = identity;
    for (int i = steps_num - 1; i >= 0; i--) {
        int offset_prev = 1 << i;
        int offset = 1 << (i + 1);

        #pragma omp parallel
        #pragma omp for
        for (int j = 0; j < arr_len; j += offset) {
            int right = arr_len - j - 1;
            int left = arr_len - j - offset_prev - 1;

            if (left >= 0) {
                int temp = out_arr[left];
                out_arr[left] = out_arr[right];
                out_arr[right] = (*operator)(out_arr[right], temp);
            }
        }
    }
}

void split(int* in_arr, int* out_arr, int arr_len, int bit_index) {
    inline int is_zero(int x) {
        return !(x & (1 << bit_index));
    }

    inline int is_one(int x) {
        return x & (1 << bit_index);
    }

    inline int add(int x, int y) {
        return x + y;
    }

    int* zero_flags = malloc(sizeof(int) * arr_len);
    int* one_flags = malloc(sizeof(int) * arr_len);

    #pragma omp parallel sections 
    {
        #pragma omp section
        map(in_arr, zero_flags, arr_len, &is_zero);
        
        #pragma omp section
        map(in_arr, one_flags, arr_len, &is_one);
    }
    
    int* zero_index = malloc(sizeof(int) * arr_len);
    int* one_index = malloc(sizeof(int) * arr_len);

    #pragma omp parallel sections 
    {
        #pragma omp section
        prescan(zero_flags, zero_index, arr_len, 0, &add);
        
        #pragma omp section
        prescan(one_flags, one_index, arr_len, 0, &add);
    }

    int one_index_offset = zero_index[arr_len - 1] + zero_flags[arr_len - 1];
    #pragma omp parallel
    #pragma omp for
    for (int i = 0; i < arr_len; i++) {
        if (zero_flags[i]) {
            out_arr[zero_index[i]] = in_arr[i];
        } else {
            out_arr[one_index[i] + one_index_offset] = in_arr[i];
        }
    }

    free(zero_flags);
    free(one_flags);
    free(zero_index);
    free(one_index);
}
