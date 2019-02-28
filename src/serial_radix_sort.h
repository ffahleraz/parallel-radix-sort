#ifndef SERIAL_RADIX_SORT_H_
#define SERIAL_RADIX_SORT_H_

int get_max(int* arr, int arr_len);
void counting_sort(int* arr, int arr_len, int exp);
void serial_radix_sort(int* in_arr, int* out_arr, int arr_len);

#endif  // SERIAL_RADIX_SORT_H_