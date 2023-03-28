#include <stdlib.h>

void __attribute__((constructor)) mod_preinit() {
    printf("Hello World\n");
}