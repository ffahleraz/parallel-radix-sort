#include <stdio.h>
#include <stdlib.h>
#include "parallel_helpers.h"
#include "helpers.h"

int main(int argc, char *argv[]) {
    int arr_len;

    if (argc == 2) {
        arr_len = strtol(argv[1], NULL, 10);
    } else {
        printf("Usage: ./radix_sort <array_length>\n");
        return 1;
    }

    // Generate random array of length arr_len
    printf("Generating input of size %d...\n", arr_len);
    
    int* arr = malloc(sizeof(int) * arr_len);
    generate_random(arr, arr_len);
    print(arr, arr_len);

    // printf("Splitting...\n");

    // Split result
    // int* res = malloc(sizeof(int) * arr_len);
    // split(arr, res, arr_len, 0);
    // for (int i = 0; i < arr_len; i ++)
    //     printf("%d ", res[i]);
    // printf("\n");

    free(arr);
    // free(res);

    return 0;
}
