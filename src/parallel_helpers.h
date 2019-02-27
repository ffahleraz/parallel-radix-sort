#ifndef PARALLEL_HELPERS_H_
#define PARALLEL_HELPERS_H_

void map(int* in_arr, int* out_arr, int arr_len, int (*pred)(int));
void prefix_sum(int* in_arr, int* out_arr, int arr_len);
// void filter(int* in_arr, int* out_arr, int arr_len, int (*pred)(int));

#endif  // PARALLEL_HELPERS_H_