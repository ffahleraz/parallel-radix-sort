#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "serial_radix_sort.h"
#include "parallel_radix_sort.h"
#include "helpers.h"

void parallel_radix_sort_test(int* arr, int arr_len);
void serial_radix_sort_test(int* arr, int arr_len);

int main(int argc, char *argv[]) {
    int arr_len;
    int* arr;

    if (argc == 2) {
        arr_len = strtol(argv[1], NULL, 10);
    } else {
        printf("Usage: ./radix_sort <array_length>\n");
        return 1;
    }

    printf("Generating input of size %d...\n", arr_len);
    arr = malloc(sizeof(int) * arr_len);
    generate_random(arr, arr_len);
    // print(arr, arr_len);

    parallel_radix_sort_test(arr, arr_len);
    serial_radix_sort_test(arr, arr_len);

    free(arr);
    return 0;
}

void parallel_radix_sort_test(int* arr, int arr_len) {
    clock_t t;
    double total = 0;
    int* result_arr = malloc(sizeof(int) * arr_len);
    
    for (int i = 1; i <= 3; i++) {
        printf("\nSorting in parallel...\n");
        t = clock();

        memcpy(result_arr, arr, arr_len * sizeof(int));
        parallel_radix_sort(result_arr, arr_len);
        // print(result_arr, arr_len);
        
        t = clock() - t;
        double sort_time = (double) t / (CLOCKS_PER_SEC / 1000);
        total += sort_time;
        printf("Parallel radix sort - %d finished, time taken: %f ms\n", i, sort_time);
    }

    free(result_arr);
    printf("\nParallel radix sort average time: %f ms\n", total / 3);
}

void serial_radix_sort_test(int* arr, int arr_len) {
    clock_t t;
    double total = 0;
    int* result_arr = malloc(sizeof(int) * arr_len);
    
    for (int i = 1; i <= 3; i++) {
        printf("\nSorting in serial...\n");
        t = clock();

        memcpy(result_arr, arr, arr_len * sizeof(int));
        serial_radix_sort(result_arr, arr_len);
        // print(result_arr, arr_len);
        
        t = clock() - t;
        double sort_time = (double) t / (CLOCKS_PER_SEC / 1000);
        total += sort_time;
        printf("Serial radix sort - %d finished, time taken: %f ms\n", i, sort_time);
    }

    free(result_arr);
    printf("\nSerial radix sort average time: %f ms\n", total / 3);
}
