#include <stdio.h>
#include <stdlib.h>
#include "serial_radix_sort.h"
#include "parallel_radix_sort.h"
#include "helpers.h"

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
    print(in_arr, arr_len);

    printf("\nSorting in serial...\n");
    serial_radix_sort(in_arr, out_arr, arr_len);
    print(out_arr, arr_len);

    free(in_arr);
    free(out_arr);

    return 0;
}
