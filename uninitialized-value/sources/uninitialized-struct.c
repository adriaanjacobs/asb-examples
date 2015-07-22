#include <stdio.h>
#include <stdlib.h>

/*
 * both, gcc and clang will find this problem.
 */

struct s 
{
 int a;
 int b;
}; 

int main(int argc, char** argv) {

  struct s test;
  printf("%d\n", test.a); 
  return 0;
}
