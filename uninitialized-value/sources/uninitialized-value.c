#include <stdio.h>
#include <stdlib.h>

/*
 * both, gcc and clang will find this problem.
 */

void intermediatefunction()
{
  int x[3];
  printf("Uninitialized value: %d\n", x[0]);
  //int y;
  //printf("Uninitialized value y: %d\n", y);

}
  

int main(int argc, char** argv) {

  intermediatefunction(); 
  return 0;
}
