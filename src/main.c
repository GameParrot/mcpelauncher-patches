#include <stdlib.h>

int nestedchild() {
    printf("enter nestedchild\n");
    printf("exit nestedchild\n");
    return 6;
}


int child() {
    printf("enter child\n");
    int r = nestedchild();
    printf("nestedchild:");
    putc('0' + r);
    printf("\n");
    printf("exit child\n");
    return 3;
}

void __attribute__((constructor)) mod_preinit() {
    printf("Hello World\n");
    int r = child();
    printf("child:");
    putc('0' + r);
    printf("\n");
}