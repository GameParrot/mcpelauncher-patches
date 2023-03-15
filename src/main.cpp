#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <playapi/api.h>
#include <playapi/device_info.h>
#include <playapi/login.h>
#include <playapi/file_login_cache.h>
#include <playapi/checkin.h>
#include <playapi/util/config.h>
#include <string>
#include <jnivm/vm.h>
#include <jnivm/env.h>
#include <fstream>
#include <sstream>

#ifdef NDEBUG
#define printf(...)
#endif

void (*mcpelauncher_preinithook)(const char*sym, void*val, void **orig);

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
    auto h = dlopen("libmcpelauncher_mod.so", 0);
    if(!h) {
        return;
    }
    mcpelauncher_preinithook = (decltype(mcpelauncher_preinithook)) dlsym(h, "mcpelauncher_preinithook");
    dlclose(h);
    mcpelauncher_preinithook("__cmsg_nxthdr", (void*)+[](void* msgh, void* cmsg) -> void* {
        return nullptr;
    }, nullptr);
}
