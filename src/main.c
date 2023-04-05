#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

int nestedchild() {
    printf("enter nestedchild\n");
    printf("exit nestedchild\n");
    return 6;
}


int child() {
    printf("enter child\n");
    int r = nestedchild();
    printf("nestedchild:");
    putchar('0' + r);
    printf("\n");
    printf("exit child\n");
    return 3;
}

void empty_variadic(int n, ...) {

}

void pseudo_printf(int n, ...) {
    va_list args;
    va_start(args, n);
    for(int i = 0; i < n; i++) {
        const char* str = va_arg(args, const char*);
        if(str) {
            if(i) {
                putchar('\n');
            }
            puts(str);
        }
    }
    va_end(args);
    putchar('\n');
}
int add3(int a, int b, int c) {
    return a + b + c;
}

void __attribute__((constructor)) mod_preinit() {
    printf("Hello World\n");
    int r = child();
    printf("child:");
    putchar('0' + r);
    printf("\n");
    //pseudo_printf(4, "Hello", "World", "From", "M1");
    putchar('0' + add3(2, 2, 5));
    printf("\n");
    printf("empty_variadic(0)");
    empty_variadic(0);
    printf("pseudo_printf(0)");
    pseudo_printf(0);
    printf("pseudo_printf(1, \"Hello Wrld\")");
    pseudo_printf(1, "Hello Wrld");
}