#include <stdio.h>
#include <stdlib.h>

/*
 * both, gcc and clang will find this problem.
 */

int main(int argc, char** argv) {
  int x;
  int rndseed;
  int a;

  srand(rndseed);
  printf("%d\n", rndseed);
  printf("%d\n", rand());

  return 0;
}
