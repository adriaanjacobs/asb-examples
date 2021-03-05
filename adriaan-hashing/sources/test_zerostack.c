#include <stdio.h>
#include <string.h>

int size = 69;

// basepointer
// return address
// 
// a
//
//
// stackpointer

__attribute__((always_inline)) inline void testfunc() {
    register char * const basepointer  asm("ebp");
    register char * const stackpointer asm("esp");
    printf("INSIDE:: bp: %p, sp: %p\n", basepointer, stackpointer);    
}


int main() {
    register char * const basepointer  asm("ebp");
    register char * const stackpointer asm("esp");

    printf("BEFORE:: bp: %p, sp: %p\n", basepointer, stackpointer);
    testfunc();
    printf("AFTER:: bp: %p, sp: %p\n", basepointer, stackpointer);

    printf("%d\n", size);
}