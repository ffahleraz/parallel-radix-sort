#include <stdio.h>
#include <stdlib.h>
#include "parallel_helpers.h"

// Random number generator
int is_one(int);
int add(int, int);
void rng(int*, int);

int main(int argc, char *argv[]) {
    int arr_len;

    if (argc == 2) {
        arr_len = strtol(argv[1], NULL, 10);
    } else {
        printf("Usage: ./radix_sort <array_length>\n");
        return 1;
    }

    // int arr[arr_len];
    // rng(arr, arr_len);

    int arr[] = {5,7,3,1,4,2,7,2};
    arr_len = 8;

    // Original
    for (int i = 0; i < arr_len; i ++)
        printf("%d ", arr[i]);
    printf("\n");

    // // Map
    // map(arr, arr, arr_len, &is_one);
    // for (int i = 0; i < arr_len; i ++)
    //     printf("%d ", arr[i]);
    // printf("\n");

    // printf("prefix sum:\n");

    // Prefix sum
    int result[8];
    split(arr, result, arr_len, 0);
    for (int i = 0; i < arr_len; i ++)
        printf("%d ", result[i]);
    printf("\n");

    return 0;
}

int is_one(int x) {
    return x & 1;
}

int add(int a, int b) {
    return a + b;
}

void rng(int* arr, int len) {
    int seed = 13516095;
    srand(seed);
    for(long i = 0; i < len; i++) {
        arr[i] = (int)rand();
    }
}