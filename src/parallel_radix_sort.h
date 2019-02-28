#ifndef PARALLEL_RADIX_SORT_H_
#define PARALLEL_RADIX_SORT_H_

void map(int* in_arr, int* out_arr, int arr_len, int (*predicate)(int));
void prescan(int* in_arr, int* out_arr, int arr_len, int identity, int (*operator)(int, int));
void split(int* in_arr, int* out_arr, int arr_len, int bit_index);

#endif  // PARALLEL_RADIX_SORT_H_