// stack_examine.c
// Compile: gcc -fno-stack-protector -g -O0 stack_examine.c -o stack_examine
// Run under GDB to examine addresses

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void examine_stack(void){
    char buffer[64]; 
    int x = 0xDEADBEEF;
    int y = 0xCAFEBABE; 
    // TODO: Print the address of each local variable
    printf("Address of buffer[]: %p\n", (void *)buffer);
    printf("Address of x: %p\n", (void *)&x);
    printf("Address of y: %p\n", (void *)&y);
    // TODO: Print the address of the saved RBP (hint: where is RBP pointing?)
    register void *rbp asm("rbp");
    printf("Saved RBP location: %p\n", (void *)rbp);
    // TODO: Print the address of the return address (hint: RBP + 8 on x86-64)
    printf("Address of return address %p\n", rbp + 8);
    for(int i = 0; i < 64; i++){
       buffer[i] = 'A'; 
    }
    puts("Examine in GDB now. Hit enter to continue...");
    getchar();
}

int main(void){
    printf("main() is at address: %p\n", (void *)main);
    examine_stack();
    puts("Returned safely to main");
    return EXIT_SUCCESS;
}
