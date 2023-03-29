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
    char buf[2];
    buf[0] = '0' + r;
    buf[1] = (char)0;
    printf(buf);
    printf("\n");
    printf("exit child\n");
    return 3;
}

void __attribute__((constructor)) mod_preinit() {
    printf("Hello World\n");
    int r = child();
    printf("child:");
    char buf[2];
    buf[0] = '0' + r;
    buf[1] = (char)0;
    printf(buf);
    printf("\n");
}