#include <stdio.h>
#include <stdlib.h>
#include "parallel_helpers.h"

// Random number generator
void rng(int*, int);

int main(int argc, char *argv[]) {
    int arr_len;

    if (argc == 2) {
        arr_len = strtol(argv[1], NULL, 10);
    } else {
        printf("Usage: ./radix_sort <array_length>\n");
        return 1;
    }

    // Generate random array of length arr_len
    int arr[arr_len];
    rng(arr, arr_len);

    // Original
    for (int i = 0; i < arr_len; i ++)
        printf("%d ", arr[i]);
    printf("\n");

    // Split result
    int result[arr_len];
    split(arr, result, arr_len, 0);
    for (int i = 0; i < arr_len; i ++)
        printf("%d ", result[i]);
    printf("\n");

    return 0;
}

void rng(int* arr, int len) {
    int seed = 13516095;
    srand(seed);
    for(long i = 0; i < len; i++) {
        arr[i] = (int)rand();
    }
}