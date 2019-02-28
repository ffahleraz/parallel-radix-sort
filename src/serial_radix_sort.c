#include <string.h>
#include "serial_radix_sort.h"

int get_max(int* arr, int arr_len) { 
    int mx = arr[0]; 
    for (int i = 1; i < arr_len; i++) 
        if (arr[i] > mx) 
            mx = arr[i]; 
    return mx; 
} 
  
void counting_sort(int* arr, int arr_len, int exp) {
    int output[arr_len];
    int i, count[10] = {0}; 
  
    for (i = 0; i < arr_len; i++) 
        count[(arr[i] / exp) % 10]++; 
  
    for (i = 1; i < 10; i++) 
        count[i] += count[i - 1]; 
  
    for (i = arr_len - 1; i >= 0; i--) { 
        output[count[(arr[i] / exp) % 10] - 1] = arr[i]; 
        count[(arr[i] / exp) % 10]--; 
    } 

    for (i = 0; i < arr_len; i++) 
        arr[i] = output[i]; 
} 
  
void serial_radix_sort(int* arr, int arr_len) {
    int m = get_max(arr, arr_len);
    for (int exp = 1; (m / exp) > 0; exp *= 10) 
        counting_sort(arr, arr_len, exp);
}