#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <netdb.h>

void (*mcpelauncher_preinithook)(const char*sym, void*val, void **orig);

void*_ZNK11AppPlatform12isLANAllowedEv;
void*__ZNK11AppPlatform12isLANAllowedEv;
int bp = 8;
int patchId = 0;

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
    auto h = dlopen("libmcpelauncher_mod.so", 0);

    mcpelauncher_preinithook = (decltype(mcpelauncher_preinithook)) dlsym(h, "mcpelauncher_preinithook");

    mcpelauncher_preinithook("getaddrinfo", (void*)+[](const char *node, const char *service, const addrinfo *hints, addrinfo **res) -> int {
        if (node) {
            if (strcmp(node, "geo.hivebedrock.network") == 0) {
                patchId++;
                std::thread([=]() {
                    int patchIdOld = patchId;
                    bp = 1;
                    sleep(3);
                    if (patchIdOld == patchId) {
                        bp = 8;
                    }
                }).detach();
            }
        }
        return getaddrinfo(node, service, hints, res);
    }, nullptr);

    
    __ZNK11AppPlatform12isLANAllowedEv = (void*)+[](void*** t) -> bool {
        auto mc = dlopen("libminecraftpe.so", 0);
        
        auto appPlat = (void**)dlsym(mc, "_ZTV11AppPlatform");
        auto raw = &appPlat[2];
        auto othervt = *t;
        auto _ZNK11AppPlatform19supportsFilePickingEv = (void**)dlsym(mc, "_ZNK11AppPlatform19supportsFilePickingEv");
        auto _ZNK11AppPlatform17supportsScriptingEv = (void**)dlsym(mc, "_ZNK11AppPlatform17supportsScriptingEv");
        auto _ZNK11AppPlatform25getPlatformUIScalingRulesEv = (void**)dlsym(mc, "_ZNK11AppPlatform25getPlatformUIScalingRulesEv");
        auto _ZNK11AppPlatform23getMaxSimRadiusInChunksEv = (void**)dlsym(mc, "_ZNK11AppPlatform23getMaxSimRadiusInChunksEv");
        auto _ZNK11AppPlatform10getEditionEv = (void**)dlsym(mc, "_ZNK11AppPlatform10getEditionEv");
        auto _ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv = (void**)dlsym(mc, "_ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv");
        auto _ZNK11AppPlatform16getBuildPlatformEv = (void**)dlsym(mc, "_ZNK11AppPlatform16getBuildPlatformEv");

        for(int i = 0; raw[i] && raw[i] != (void*)0xffffffffffffffe8; i++) {
            if(raw[i] == _ZNK11AppPlatform19supportsFilePickingEv) {
                othervt[i] = (void*) +[](void*t) -> bool {
                    return true;
                };
            }
            if(raw[i] == _ZNK11AppPlatform17supportsScriptingEv) {
                othervt[i] = (void*) +[](void*t) -> bool {
                    return true;
                };
            }
            if(raw[i] == _ZNK11AppPlatform25getPlatformUIScalingRulesEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    return 0;
                };
            }
            if(raw[i] == _ZNK11AppPlatform23getMaxSimRadiusInChunksEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    return 12;
                };
            }
            if(raw[i] == _ZNK11AppPlatform10getEditionEv) {
                othervt[i] = (void*) +[](void*t) -> std::string {
                    return "win10";
                };
            }
            if(raw[i] == _ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    return 8;
                };
            }
            if(raw[i] == _ZNK11AppPlatform16getBuildPlatformEv) {
                othervt[i] = (void*) +[](void*t) -> int {
                    return bp;
                };
            }
            if(othervt[i] == __ZNK11AppPlatform12isLANAllowedEv) {
                othervt[i] = _ZNK11AppPlatform12isLANAllowedEv;
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
