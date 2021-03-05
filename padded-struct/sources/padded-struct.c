#include <stdio.h>
#include <unistd.h>

/*
 * clang -fsanitize=memory will find this behavior!
 */

struct padded_struct {
  char ch1; // 1 byte
  // 3 padding bytes
  int i1;   // 4 bytes on 64bits system
  int i2;   // 4 bytes
};

int main(int argc, char *argv[]) {
  struct padded_struct foo;
    
  foo.ch1 = 'a';
  foo.i1 = 0;
  foo.i2 = 1;
    
  printf("sizeof padded_struct is %ld\n", sizeof(foo)); 
  /* sizeof padded_struct is 12 */
    
  write(2, &foo, sizeof(foo));
  /* Argument mismatch triggered if ASLR is enabled */
  return 0;
}
