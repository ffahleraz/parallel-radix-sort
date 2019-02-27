#include <stdio.h>
#include <stdlib.h>
#include "parallel_helpers.h"

// Random number generator
int is_one(int x);
void rng(int* arr, int len);

int main(int argc, char *argv[]) {
    int arr_len;

    if (argc == 2) {
        arr_len = strtol(argv[1], NULL, 10);
    } else {
        printf("Usage: ./radix_sort <array_length>\n");
        return 1;
    }

    int arr[arr_len];
    rng(arr, arr_len);

    // Original
    for (int i = 0; i < arr_len; i ++)
        printf("%d ", arr[i]);
    printf("\n");

    // Map
    map(arr, arr, arr_len, &is_one);
    for (int i = 0; i < arr_len; i ++)
        printf("%d ", arr[i]);
    printf("\n");

    printf("prefix sum:\n");

    // Prefix sum
    prefix_sum(arr, arr, arr_len);
    for (int i = 0; i < arr_len; i ++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}

int is_one(int x) {
    return x & 1;
}

void rng(int* arr, int len) {
    int seed = 13516095;
    srand(seed);
    for(long i = 0; i < len; i++) {
        arr[i] = (int)rand();
    }
}