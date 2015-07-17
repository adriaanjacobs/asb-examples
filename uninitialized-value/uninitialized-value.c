#include <stdio.h>
#include <stdlib.h>

/*
 * both, gcc and clang will find this problem.
 */

int* print_uninit()
{
  int x[10];
  printf("%p\n", x);
  printf("%d\n", x[4]);
  return &x;
}

int main(int argc, char** argv) {

  int *p=print_uninit();
  printf("the value is %d\n",p[4]);
  return 0;
}
