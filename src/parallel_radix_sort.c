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

void split(int* arr, int arr_len, int digit_index) {
    int divisor = pow(10, digit_index);
    int* flags[10];
    int* scatter_index[10];
    int* result = malloc(sizeof(int) * arr_len * 10);

    inline int add(int x, int y) {
        return x + y;
    }

    // #pragma omp parallel
    // #pragma omp for
    for (int i = 0; i < 10; i++) {
        inline int is_digit(int x) {
            return (x / divisor) % 10 == i;
        }

        flags[i] = malloc(sizeof(int) * arr_len * 10);
        scatter_index[i] = malloc(sizeof(int) * arr_len * 10);
        
        memcpy(flags[i], arr, arr_len * sizeof(int));
        map(flags[i], arr_len, &is_digit);
        
        memcpy(scatter_index[i], flags[i], arr_len * sizeof(int));
        prescan(scatter_index[i], arr_len, 0, &add);

        print(flags[i], arr_len);
        print(scatter_index[i], arr_len);
    }

    // #pragma omp parallel
    // #pragma omp for
    for (int i = 0; i < 10; i++) {
        
        memcpy(flags[i], arr, arr_len * sizeof(int));
        map(flags[i], arr_len, &is_digit);
        
        memcpy(scatter_index[i], flags[i], arr_len * sizeof(int));
        prescan(scatter_index[i], arr_len, 0, &add);

        print(flags[i], arr_len);
        print(scatter_index[i], arr_len);
    }

    for (int i = 0; i < 10; i++) {
        free(flags[i]);
        free(scatter_index[i]);
    }

    // #pragma omp parallel sections 
    // {
    //     #pragma omp section
    //     map(in_arr, zero_flags, arr_len, &is_zero);
        
    //     #pragma omp section
    //     map(in_arr, one_flags, arr_len, &is_one);
    // }
    
    // int* zero_index = malloc(sizeof(int) * arr_len);
    // int* one_index = malloc(sizeof(int) * arr_len);

    // inline int add(int x, int y) {
    //     return x + y;
    // }

    // #pragma omp parallel sections 
    // {
    //     #pragma omp section
    //     prescan(zero_flags, zero_index, arr_len, 0, &add);
        
    //     #pragma omp section
    //     prescan(one_flags, one_index, arr_len, 0, &add);
    // }

    // int one_index_offset = zero_index[arr_len - 1] + zero_flags[arr_len - 1];
    // #pragma omp parallel
    // #pragma omp for
    // for (int i = 0; i < arr_len; i++) {
    //     if (zero_flags[i]) {
    //         out_arr[zero_index[i]] = in_arr[i];
    //     } else {
    //         out_arr[one_index[i] + one_index_offset] = in_arr[i];
    //     }
    // }

    // free(zero_flags);
    // free(one_flags);
    // free(zero_index);
    // free(one_index);
}

void parallel_radix_sort(int* arr, int arr_len) {
    for (int i = 0; i < sizeof(int); i++) {
        // split()
    }
}
