#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parallel_helpers.h"

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
    out_arr[0] += identity;
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

void split(int* in_arr, int* out_arr, int arr_len) {
    // Functions for different map uses
    inline int is_zero(int x) {
        return (x & 1) << sizeof(int);
    }

    inline int is_one(int x) {
        return x & 1;
    }

    inline int add(int x, int y) {
        return x + y;
    }

    int zero_flags[arr_len];
    int one_flags[arr_len];

    int zero_index[arr_len];
    int one_index[arr_len];

    int index[arr_len];

    map(in_arr, zero_flags, arr_len, &is_zero);
    map(in_arr, one_flags, arr_len, &is_one);

    prescan(zero_flags, zero_index, arr_len, 0, &add);
    prescan(one_flags, one_index, arr_len, zero_index[arr_len - 1], &add);


}
