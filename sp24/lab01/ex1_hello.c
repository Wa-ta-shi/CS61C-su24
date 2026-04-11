#include <stdio.h>

int main() {
  printf("Hello World\n");

  int my_var = 20;
  int* my_var_p;
  my_var_p = &my_var;

  printf("Address of my_var: %p\n", my_var_p);
  printf("Address of my_var: %p\n", &my_var);
  printf("Address of my_var_p: %p\n", &my_var_p);

  *my_var_p += 2;

  printf("my_var: %d\n", my_var);
  printf("my_var: %d\n", *my_var_p);
  return 0;
}
