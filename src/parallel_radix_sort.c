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

void split(int* arr, int arr_len, int divisor) {    
    int* flags[10];
    int* scatter_index[10];
    int scatter_offset[10];
    
    int* digits = malloc(sizeof(int) * arr_len * 10);
    int* result = malloc(sizeof(int) * arr_len * 10);

    // Save each number's digit_index-th digit
    inline int get_digit(int x) {
        return (x / divisor) % 10;
    }
    memcpy(digits, arr, arr_len * sizeof(int));
    map(digits, arr_len, &get_digit);

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
    }

    // Save scatter index offset for each digit
    scatter_offset[0] = 0;
    for (int i = 1; i < 10; i++) {
        scatter_offset[i] = scatter_offset[i - 1] + scatter_index[i - 1][arr_len - 1] + (digits[arr_len - 1] == i - 1);
    }

    // Scatter each input to their corresponding index
    #pragma omp parallel
    #pragma omp for
    for (int i = 0; i < arr_len; i++) {
        result[scatter_index[digits[i]][i] + scatter_offset[digits[i]]] = arr[i];
    }

    // Copy result to input
    memcpy(arr, result, arr_len * sizeof(int));

    // Free stuff
    for (int i = 0; i < 10; i++) {
        free(flags[i]);
        free(scatter_index[i]);
    }
    free(result);
}

void parallel_radix_sort(int* arr, int arr_len) {
    inline int max(int x, int y) {
        return x > y ? x : y;
    }
    int max_element = reduce(arr, arr_len, &max);
    for (int divisor = 1; max_element / divisor > 0; divisor *= 10) {
        split(arr, arr_len, divisor);
    }
}
