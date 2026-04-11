#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  // Creates an integer with value 5
  // Note: int16_t is a numerical datatype that takes up 2 bytes of memory
  int16_t x = 5;

  // TODO: allocate an int16_t array of size 4
  // Hint: C represents arrays as a pointer to the first element
  // Hint: one malloc call allocates memory for the entire array
  //       it works because the elements are stored side by side
  int16_t* some_array = malloc(4 * sizeof(int16_t));
  printf("address of the start of the array: %p\n", some_array);

  // TODO: compute the address of the element at index 2 (0-indexed)
  int16_t* ptr_to_idx_2 = some_array + 2;
  printf("address of index 2: %p\n", ptr_to_idx_2);

  // TODO: store the value 10 at index 2, using ptr_to_idx_2
  *ptr_to_idx_2 = 10;

  // TODO: print the value at index 2
  // Hint: this blank should be the same as the previous blank
  //       please don't hard code 10
  printf("value at index 2: %d\n", *ptr_to_idx_2);

  free(some_array);

  // int8_t* arr1 = malloc(4 * sizeof(int8_t));
  // int32_t* arr2 = malloc(4 * sizeof(int32_t));
  // double* arr3 = malloc(4 * sizeof(double));
  //
  // printf("int8_t 数组:\n");
  // printf("&arr1[0] = %p\n", &arr1[0]);
  // printf("&arr1[2] = %p\n", &arr1[2]);
  // printf("差值: %td 字节\n\n", (char*)&arr1[2] - (char*)&arr1[0]);
  //
  // printf("int32_t 数组:\n");
  // printf("&arr2[0] = %p\n", &arr2[0]);
  // printf("&arr2[2] = %p\n", &arr2[2]);
  // printf("差值: %td 字节\n\n", (char*)&arr2[2] - (char*)&arr2[0]);
  //
  // printf("double 数组:\n");
  // printf("&arr3[0] = %p\n", &arr3[0]);
  // printf("&arr3[2] = %p\n", &arr3[2]);
  // printf("差值: %td 字节\n\n", (char*)&arr3[2] - (char*)&arr3[0]);
  //
  // free(arr1);
  // free(arr2);
  // free(arr3);

  return 0;
}
