#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

void print(int* arr, int arr_len) {
    for (int i = 0; i < arr_len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void generate_random(int* arr, int arr_len) {
    int seed = 13516095;
    srand(seed);
    for (int i = 0; i < arr_len; i++) {
        arr[i] = (int) rand();
    }
}