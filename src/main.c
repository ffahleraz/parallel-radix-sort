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

    printf("Generating input...\n");

    // Generate random array of length arr_len
    int* arr = malloc(sizeof(int) * arr_len);
    rng(arr, arr_len);
    // for (int i = 0; i < arr_len; i ++)
    //     printf("%d ", arr[i]);
    // printf("\n");

    printf("Splitting...\n");

    // Split result
    int* res = malloc(sizeof(int) * arr_len);
    split(arr, res, arr_len, 0);
    // for (int i = 0; i < arr_len; i ++)
    //     printf("%d ", res[i]);
    // printf("\n");

    free(arr);
    free(res);

    return 0;
}

void rng(int* arr, int len) {
    int seed = 13516095;
    srand(seed);
    for(long i = 0; i < len; i++) {
        arr[i] = (int)rand();
    }
}