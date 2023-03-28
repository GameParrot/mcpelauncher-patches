#include <stdlib.h>

int nestedchild() {
    printf("enter nestedchild\n");
    printf("exit nestedchild\n");
    return 23;
}


int child() {
    printf("enter child\n");
    int r = nestedchild();
    printf("nestedchild: %d\n", r);
    printf("exit child\n");
    return 54;
}

void __attribute__((constructor)) mod_preinit() {
    printf("Hello World\n");
    int r = child();
    printf("child: %d\n", r);
}