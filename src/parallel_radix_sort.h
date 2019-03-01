#ifndef PARALLEL_RADIX_SORT_H_
#define PARALLEL_RADIX_SORT_H_

void map(int* arr, int arr_len, int (*predicate)(int));
int reduce(int* arr, int arr_len, int (*operator)(int, int));
void prescan(int* arr, int arr_len, int identity, int (*operator)(int, int));
void split(int* arr, int arr_len, int bit_index);
void parallel_radix_sort(int* arr, int arr_len);

#endif  // PARALLEL_RADIX_SORT_H_