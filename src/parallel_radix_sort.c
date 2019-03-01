#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "helpers.h"
#include "parallel_radix_sort.h"

void map(int* arr, int arr_len, int (*predicate)(int)) {
    #pragma omp parallel
    #pragma omp for
    for (int i = 0; i < arr_len; i++) {
        arr[i] = (*predicate)(arr[i]);
    }
}

int reduce(int* arr, int arr_len, int (*operator)(int, int)) {
    int steps_num = (int) ceil(log(arr_len) / log(2));
    int* temp = malloc(sizeof(int) * arr_len * 10);
    memcpy(temp, arr, arr_len * sizeof(int));

    for (int i = 0; i < steps_num; i++) {
        int offset_prev = 1 << i;
        int offset = 1 << (i + 1);

        #pragma omp parallel
        #pragma omp for
        for (int j = 0; j < arr_len; j += offset) {
            int right = arr_len - j - 1;
            int left = arr_len - j - offset_prev - 1;

            if (left >= 0) {
                temp[right] = (*operator)(temp[left], temp[right]);
            }
        }
    }

    return temp[arr_len - 1];
}

void prescan(int* arr, int arr_len, int identity, int (*operator)(int, int)) {
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
                arr[right] = (*operator)(arr[left], arr[right]);
            }
        }
    }

    // Down sweep
    arr[arr_len - 1] = identity;
    for (int i = steps_num - 1; i >= 0; i--) {
        int offset_prev = 1 << i;
        int offset = 1 << (i + 1);

        #pragma omp parallel
        #pragma omp for
        for (int j = 0; j < arr_len; j += offset) {
            int right = arr_len - j - 1;
            int left = arr_len - j - offset_prev - 1;

            if (left >= 0) {
                int temp = arr[left];
                arr[left] = arr[right];
                arr[right] = (*operator)(arr[right], temp);
            }
        }
    }
}

void split(int* arr, int arr_len, int bit_index) {
    inline int is_zero(int x) {
        return !((x >> bit_index) & 1);
    }

    inline int is_one(int x) {
        return (x >> bit_index) & 1;
    }

    inline int add(int x, int y) {
        return x + y;
    }

    int* zero_flags = malloc(sizeof(int) * arr_len);
    int* one_flags = malloc(sizeof(int) * arr_len);
    memcpy(zero_flags, arr, arr_len * sizeof(int));
    memcpy(one_flags, arr, arr_len * sizeof(int));

    #pragma omp parallel sections 
    {
        #pragma omp section
        map(zero_flags, arr_len, &is_zero);
        
        #pragma omp section
        map(one_flags, arr_len, &is_one);
    }
    
    int* zero_index = malloc(sizeof(int) * arr_len);
    int* one_index = malloc(sizeof(int) * arr_len);
    memcpy(zero_index, zero_flags, arr_len * sizeof(int));
    memcpy(one_index, one_flags, arr_len * sizeof(int));

    #pragma omp parallel sections 
    {
        #pragma omp section
        prescan(zero_index, arr_len, 0, &add);
        
        #pragma omp section
        prescan(one_index, arr_len, 0, &add);
    }

    int* result = malloc(sizeof(int) * arr_len);
    int one_index_offset = zero_index[arr_len - 1] + zero_flags[arr_len - 1];
    #pragma omp parallel
    #pragma omp for
    for (int i = 0; i < arr_len; i++) {
        if (zero_flags[i]) {
            result[zero_index[i]] = arr[i];
        } else {
            result[one_index[i] + one_index_offset] = arr[i];
        }
    }

    memcpy(arr, result, arr_len * sizeof(int));

    free(zero_flags);
    free(one_flags);
    free(zero_index);
    free(one_index);
    free(result);
}

void parallel_radix_sort(int* arr, int arr_len) {
    inline int max(int x, int y) {
        return x > y ? x : y;
    }
    int max_element = reduce(arr, arr_len, &max);

    int bit_index = 0;
    for (; max_element > 0; max_element >>= 1) {
        split(arr, arr_len, bit_index);
        bit_index++;
    }
}
