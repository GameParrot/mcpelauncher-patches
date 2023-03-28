//#include <iostream>
#include <cstdlib>

extern "C" void __attribute__((constructor)) mod_preinit() {
    // std::cout << "Hello World\n";
    // std::cout << "From android\n";
    printf("Hello World\n");
}
