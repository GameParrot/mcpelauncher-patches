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
bool lastWasIp = true;

int isValidIp4 (char *str) {
    int segs = 0;   /* Segment count. */
    int chcnt = 0;  /* Character count within segment. */
    int accum = 0;  /* Accumulator for segment. */

    /* Catch NULL pointer. */

    if (str == NULL)
        return 0;

    /* Process every character in string. */

    while (*str != '\0') {
        /* Segment changeover. */

        if (*str == '.') {
            /* Must have some digits in segment. */

            if (chcnt == 0)
                return 0;

            /* Limit number of segments. */

            if (++segs == 4)
                return 0;

            /* Reset segment values and restart loop. */

            chcnt = accum = 0;
            str++;
            continue;
        }
        /* Check numeric. */

        if ((*str < '0') || (*str > '9'))
            return 0;

        /* Accumulate and check segment. */

        if ((accum = accum * 10 + *str - '0') > 255)
            return 0;

        /* Advance other segment specific stuff and continue loop. */

        chcnt++;
        str++;
    }

    /* Check enough segments and enough characters in last segment. */

    if (segs != 3)
        return 0;

    if (chcnt == 0)
        return 0;

    /* Address okay. */

    return 1;
}

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
    auto h = dlopen("libmcpelauncher_mod.so", 0);

    mcpelauncher_preinithook = (decltype(mcpelauncher_preinithook)) dlsym(h, "mcpelauncher_preinithook");

    mcpelauncher_preinithook("getaddrinfo", (void*)+[](const char *node, const char *service, const addrinfo *hints, addrinfo **res) -> int {
        if (node) {
            if (lastWasIp) {
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
                if ((isValidIp4((char*)node) == 0) && (std::string(node).find(":") == std::string::npos)) {
                    printf("Invalid\n");
                    lastWasIp = false;
                }
                printf("%s\n%d\n", node, lastWasIp);
            }
            if (strcmp(node, "play.inpvp.net") == 0) {
                bp = 8;
            } else if (strcmp(node, "mco.lbsg.net") == 0) {
                bp = 8;
            } else if (strcmp(node, "mco.mineplex.com") == 0) {
                bp = 8;
            } else if (strcmp(node, "mco.cubecraft.net") == 0) {
                bp = 8;
            } else if (strcmp(node, "play.galaxite.net") == 0) {
                bp = 8;
            } else if (strcmp(node, "play.pixelparadise.gg") == 0) {
                bp = 8;
            }
            if ((isValidIp4((char*)node) != 0) || std::string(node).find(":") != std::string::npos) {
                printf("valid\n");
                lastWasIp = true;
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
