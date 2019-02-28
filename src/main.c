#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "serial_radix_sort.h"
#include "parallel_radix_sort.h"
#include "helpers.h"

void serial_radix_sort_test(int* in_arr, int* out_arr, int arr_len);

int main(int argc, char *argv[]) {
    int arr_len;
    int* in_arr;
    int* out_arr;

    if (argc == 2) {
        arr_len = strtol(argv[1], NULL, 10);
    } else {
        printf("Usage: ./radix_sort <array_length>\n");
        return 1;
    }

    printf("Generating input of size %d...\n", arr_len);
    in_arr = malloc(sizeof(int) * arr_len);
    out_arr = malloc(sizeof(int) * arr_len);
    generate_random(in_arr, arr_len);

    serial_radix_sort_test(in_arr, out_arr, arr_len);

    free(in_arr);
    free(out_arr);

    return 0;
}

void serial_radix_sort_test(int* in_arr, int* out_arr, int arr_len) {
    clock_t t;
    double total = 0;
    for (int i = 1; i <= 3; i++) {
        printf("\nSorting in serial...\n");
        t = clock();
        serial_radix_sort(in_arr, out_arr, arr_len);
        t = clock() - t;
        double sort_time = (double) t / (CLOCKS_PER_SEC / 1000);
        total += sort_time;
        printf("Serial radix sort - %d finished, time taken: %f ms\n", i, sort_time);
    }
    printf("\nSerial radix sort average time: %f ms\n", total/3);
}
