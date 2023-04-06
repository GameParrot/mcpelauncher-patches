#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <string>

void (*mcpelauncher_preinithook)(const char*sym, void*val, void **orig);

void*_ZNK11AppPlatform12isLANAllowedEv;
void*__ZNK11AppPlatform12isLANAllowedEv;

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
    printf("Loading mod\n");
    auto h = dlopen("libmcpelauncher_mod.so", 0);
    if(!h) {
        return;
    }
    mcpelauncher_preinithook = (decltype(mcpelauncher_preinithook)) dlsym(h, "mcpelauncher_preinithook");
    
    printf("Loading mod %p\n", mcpelauncher_preinithook);
    __ZNK11AppPlatform12isLANAllowedEv = (void*)+[](void*** t) -> bool {
        printf("__ZNK11AppPlatform12isLANAllowedEv\n");
        auto mc = dlopen("libminecraftpe.so", 0);
        
        auto appPlat = (void**)dlsym(mc, "_ZTV11AppPlatform");
        auto raw = &appPlat[2];
        auto othervt = *t;
        printf("AppPlatform:\n");
        for(int i = 0; raw[i] && raw[i] != (void*)0xffffffffffffffe8; i++) {
            Dl_info data;
            printf("%p (%s)\n", raw[i], dladdr(raw[i], &data) ? data.dli_sname : "(unknown)");
        }
        printf("othervt:\n");
        for(int i = 0; othervt[i] && othervt[i] != (void*)0xffffffffffffffe8; i++) {
            Dl_info data;
            printf("%p (%s)\n", othervt[i], dladdr(othervt[i], &data) ? data.dli_sname : "(unknown)");
        }
        auto _ZNK11AppPlatform19supportsFilePickingEv = (void**)dlsym(mc, "_ZNK11AppPlatform19supportsFilePickingEv");
        auto _ZNK11AppPlatform17supportsScriptingEv = (void**)dlsym(mc, "_ZNK11AppPlatform17supportsScriptingEv");
        auto _ZNK11AppPlatform25getPlatformUIScalingRulesEv = (void**)dlsym(mc, "_ZNK11AppPlatform25getPlatformUIScalingRulesEv");
        auto _ZNK11AppPlatform23getMaxSimRadiusInChunksEv = (void**)dlsym(mc, "_ZNK11AppPlatform23getMaxSimRadiusInChunksEv");
        auto _ZNK11AppPlatform10getEditionEv = (void**)dlsym(mc, "_ZNK11AppPlatform10getEditionEv");
        auto _ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv = (void**)dlsym(mc, "_ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv");
        auto _ZNK11AppPlatform19getDefaultUIProfileEv = (void**)dlsym(mc, "_ZNK11AppPlatform19getDefaultUIProfileEv");
        
        for(int i = 0; raw[i] && raw[i] != (void*)0xffffffffffffffe8; i++) {
            if(raw[i] == _ZNK11AppPlatform19supportsFilePickingEv) {
                othervt[i] = (void*) +[](void*t) -> bool {
                    printf("_ZNK11AppPlatform19supportsFilePickingEv called\n");
                    return true;
                };
                printf("Patched _ZNK11AppPlatform19supportsFilePickingEv\n");
            }
            if(raw[i] == _ZNK11AppPlatform17supportsScriptingEv) {
                othervt[i] = (void*) +[](void*t) -> bool {
                    printf("_ZNK11AppPlatform17supportsScriptingEv called\n");
                    return true;
                };
                printf("Patched _ZNK11AppPlatform17supportsScriptingEv\n");
            }
            if(raw[i] == _ZNK11AppPlatform25getPlatformUIScalingRulesEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    printf("_ZNK11AppPlatform25getPlatformUIScalingRulesEv called\n");
                    return 0;
                };
                printf("Patched _ZNK11AppPlatform25getPlatformUIScalingRulesEv\n");
            }
            if(raw[i] == _ZNK11AppPlatform23getMaxSimRadiusInChunksEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    printf("_ZNK11AppPlatform23getMaxSimRadiusInChunksEv called\n");
                    return 12;
                };
                printf("Patched _ZNK11AppPlatform23getMaxSimRadiusInChunksEv\n");
            }
            if(raw[i] == _ZNK11AppPlatform10getEditionEv) {
                othervt[i] = (void*) +[](void*t) -> std::string {
                    printf("_ZNK11AppPlatform10getEditionEv called\n");
                    return "win10";
                };
                printf("Patched _ZNK11AppPlatform10getEditionEv\n");
            }
            if(raw[i] == _ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    printf("_ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv called\n");
                    return 8;
                };
                printf("Patched _ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv\n");
            }
            if(raw[i] == _ZNK11AppPlatform19getDefaultUIProfileEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    printf("_ZNK11AppPlatform19getDefaultUIProfileEv called\n");
                    return 0;
                };
                printf("Patched _ZNK11AppPlatform19getDefaultUIProfileEv\n");
            }
            if(othervt[i] == __ZNK11AppPlatform12isLANAllowedEv) {
                othervt[i] = _ZNK11AppPlatform12isLANAllowedEv;
                printf("Patched __ZNK11AppPlatform12isLANAllowedEv back to org\n");
            }
        }
        
        dlclose(mc);
        return true;
    };
    mcpelauncher_preinithook("_ZNK11AppPlatform12isLANAllowedEv", __ZNK11AppPlatform12isLANAllowedEv, &_ZNK11AppPlatform12isLANAllowedEv);
    dlclose(h);
}

extern "C" __attribute__ ((visibility ("default"))) void mod_init() {
}
