#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct contain_ptr 
{
  int i;
  int *p;
} __attribute__((packed));

int main()
{
  struct contain_ptr s;
  int v=1;
  s.i=v;
  s.p=&v;
  printf("size of struct = %ld\n", sizeof(s));
  write(1,&s,sizeof(s));
  return 0;
}
