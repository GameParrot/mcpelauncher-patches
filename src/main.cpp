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

int (*JNI_OnLoad_) (void*,void*);
void* pthread_getattr_np_org;

void* mremap_fake(void *old_address, size_t old_size,
                    size_t new_size, int flags, ...) {
        return MAP_FAILED;
}

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
    auto h = dlopen("libmcpelauncher_mod.so", 0);
    if(!h) {
        return;
    }
    mcpelauncher_preinithook = (decltype(mcpelauncher_preinithook)) dlsym(h, "mcpelauncher_preinithook");
    dlclose(h);
    try {
        playapi::device_info device;
        auto cachePath = getenv("GPLAY_TOKEN_CACHE_PATH");
        std::string cachePathStorage;
        auto suffix = "playapi_token_cache.conf";
        if(!cachePath) {
            std::vector<std::string> paths;
            auto cacheBasePath = getenv("XDG_CACHE_HOME");
            if(cacheBasePath) {
                paths.push_back(cacheBasePath);
            }
            auto home = getenv("HOME");
            if(home) {
                std::string shome = home;
                paths.push_back(shome + "/.cache");
                paths.push_back(shome + "/Library/Caches");
                paths.push_back(shome + "/.var/app/io.mrarm.mcpelauncher/cache");
            }
            for(auto&&path : paths) {
                auto finalPath = path + "/Minecraft Linux Launcher/Minecraft Linux Launcher UI/" + suffix;
                if(!access(finalPath.data(), R_OK)) {
                    cachePathStorage = finalPath;
                    cachePath = cachePathStorage.data();
                    break;
                }
            }
            auto localAppdata = getenv("LOCALAPPDATA");
            if(localAppdata) {
                auto finalPath = localAppdata + ("/Minecraft Linux Launcher/Minecraft Linux Launcher UI/cache/" + suffix);
                if(!access(finalPath.data(), R_OK)) {
                    cachePathStorage = finalPath;
                    cachePath = cachePathStorage.data();
                }
            }
            if(!cachePath) {
                return;
            }
        } else {
            cachePathStorage = cachePath;
        }
        auto callback_meta = []() {
            mcpelauncher_preinithook("pthread_getattr_np", (void*)+[](pthread_t th, pthread_attr_t* attr) -> int {
                return 1;
            }, &pthread_getattr_np_org);
            mcpelauncher_preinithook("pthread_attr_getstack", (void*)+[](pthread_attr_t* attr, void **stackaddr, size_t *stacksize) -> int {
                return 1;
            }, nullptr);
            mcpelauncher_preinithook("mremap", (void*)&mremap_fake, nullptr);
        };
        auto sres = cachePathStorage.rfind(suffix);
        if(sres != std::string::npos) {
            playapi::config c;
            std::ifstream fs((cachePathStorage.substr(0, sres) + "/misc.cache").data());
            if(fs.is_open()) {
                c.load(fs);
                std::ifstream f(cachePathStorage.data());
                if(f.is_open()) {
                    std::stringstream s;
                    s << f.rdbuf();
                    auto buf = s.str();
                    if(c.get("path") == cachePathStorage && c.get_long("idx") == (cachePathStorage.length() + buf.length())) {
                        callback_meta();
                        return;
                    }
                }
            }
        }
        playapi::file_login_cache cache(cachePath);
        playapi::login_api login_api(device, cache);
        login_api.set_token("l@localhost", "a");
        playapi::api api(device);
        playapi::checkin_result checkinResult;
        playapi::checkin_api checkin(device);
        checkin.add_auth(login_api)->call();
        checkinResult = checkin.perform_checkin()->call();
        api.set_checkin_data(checkinResult);
        api.set_auth(login_api)->call();
        api.details("com.mojang.minecraftpe")->call([&](playapi::proto::finsky::response::ResponseWrapper&& resp) {
            auto details = resp.payload().detailsresponse().docv2();
            if(details.details().appdetails().versionstring() == "" || !details.details().appdetails().versioncode()) {
                abort();
            }
            api.delivery("com.mojang.minecraftpe", details.details().appdetails().versioncode(), std::string())->call([&](playapi::proto::finsky::response::ResponseWrapper&& resp) {
                auto dd = resp.payload().deliveryresponse().appdeliverydata();
                auto url = (dd.has_gzippeddownloadurl() ? dd.gzippeddownloadurl() : dd.downloadurl());
                printf("Apk Url: %s\n", url.data());
                if(url == "") {
                    printf("Invalid Url\n");
                } else {
                    if(sres != std::string::npos) {
                        try {
                          std::ifstream f(cachePathStorage.data());
                          if(f.is_open()) {
                              std::stringstream s;
                              s << f.rdbuf();
                              auto buf = s.str();
                              std::ofstream fs((cachePathStorage.substr(0, sres) + "/misc.cache").data());
                              playapi::config c;
                              c.set("path", cachePathStorage);
                              c.set_long("idx", cachePathStorage.length() + buf.length());
                              c.save(fs);
                          }
                        } catch(...) {
                        }
                    }
                    callback_meta();
                }
            }, [](std::exception_ptr e) {
            });
        }, [](std::exception_ptr e) {
        });
    } catch(...) {
    }
}
