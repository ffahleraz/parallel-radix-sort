#ifndef PARALLEL_HELPERS_H_
#define PARALLEL_HELPERS_H_

void map(int* in_arr, int* out_arr, int arr_len, int (*predicate)(int));
void prescan(int* in_arr, int* out_arr, int arr_len, int identity, int (*operator)(int, int));
// void filter(int* in_arr, int* out_arr, int in_arr_len, int* out_arr_len, int (*pred)(int));

#endif  // PARALLEL_HELPERS_H_