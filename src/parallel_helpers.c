#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parallel_helpers.h"

void map(int* in_arr, int* out_arr, int arr_len, int (*pred)(int)) {
    #pragma omp parallel
    #pragma omp for
    for (int i = 0; i < arr_len; i++) {
        out_arr[i] = (*pred)(in_arr[i]);
    }
}

void prefix_sum(int* in_arr, int* out_arr, int arr_len) {
    int parts_num = (int) ceil(log(arr_len) / log(2));
    memcpy(out_arr, in_arr, arr_len * sizeof(int));

    // Up sweep
    for (int i = 0; i < parts_num; i++) {
        int offset_prev = 1 << i;
        int offset = 1 << (i + 1);

        #pragma omp parallel
        #pragma omp for
        for (int j = 0; j < arr_len; j += offset) {
            int right = arr_len - j - 1;
            int left = arr_len - j - offset_prev - 1;
            if (left >= 0) {
                out_arr[arr_len - j - 1] = out_arr[left] + out_arr[right];
            }
        }
    }
}

// void prefix_sum(int* in_arr, int* out_arr, int arr_len) {

//     memcpy(out_arr, in_arr, arr_len * sizeof(int));

//     int log_of_N_1 = (int) ceil(log(arr_len) / log(2)) - 1;
//     int two_i = 0, two_i_p1 = 0;

//     printf("%d\n", log_of_N_1);

//     // Up sweep
//     for (int i = 0; i <= log_of_N_1; i++) {
//         two_i_p1 = 1 << (i + 1);
//         two_i = 1 << i;

//         for (int j = 0; j < arr_len; j += two_i_p1) {
//             out_arr[j + two_i_p1 - 1] = out_arr[j + two_i - 1] + out_arr[j + two_i_p1 - 1];
//         }
//     }

//     // Down sweep
//     out_arr[arr_len - 1] = 0;
//     for (int i = log_of_N_1; i >= 0; i--) {
//         two_i_p1 = 1 << (i + 1);
//         two_i = 1 << i;

//         for (int j = 0; j < arr_len; j += two_i_p1) {
//             int t = out_arr[j + two_i - 1];
//             out_arr[j + two_i - 1] = out_arr[j + two_i_p1 - 1];
//             out_arr[j + two_i_p1 - 1] = t + out_arr[j + two_i_p1 - 1];
//         }
//     }
// }
