#include <Geode/Geode.hpp>
using namespace geode::prelude;

typedef int (*curl_global_init_t)(long);
typedef void* (*curl_easy_init_t)();
typedef int (*curl_easy_setopt_t)(void*, int, ...);

static curl_global_init_t original_global_init = nullptr;
static curl_easy_init_t original_easy_init = nullptr;
static curl_easy_setopt_t original_easy_setopt = nullptr;

void* hooked_curl_easy_init() {
    void* handle = original_easy_init();
    if (handle and original_easy_setopt) {
        original_easy_setopt(handle, 64, 0L);  // CURLOPT_SSL_VERIFYPEER
        original_easy_setopt(handle, 81, 0L);  // CURLOPT_SSL_VERIFYHOST
        log::debug("Disabled SSL verification for new curl handle");
    }
    return handle;
}

#ifdef GEODE_IS_WINDOWS
void* getCurlModule() {
    HMODULE libcurl = GetModuleHandleA("libcurl.dll");
    if (!libcurl) libcurl = GetModuleHandleA("libcurl-x64.dll"); // lol
    return (void*)libcurl;
}

void* getCurlFunction(void* module, const char* name) {
    return (void*)GetProcAddress((HMODULE)module, name);
}
#endif

#ifdef GEODE_IS_ANDROID
#include <dlfcn.h>

void* getCurlModule() {
    void* handle = dlopen("libcurl.so", RTLD_LAZY);
    if (!handle) handle = dlopen("libcurl.so.4", RTLD_LAZY);
    if (!handle) handle = dlopen("libcurl.so.3", RTLD_LAZY);
    return handle;
}

void* getCurlFunction(void* module, const char* name) {
    return dlsym(module, name);
}
#endif

$on_mod(Loaded) {
    void* libcurl = getCurlModule();
    if (!libcurl) return log::error(
        "libcurl not found!"
    );

    log::info("Found libcurl");

    void* easy_init_addr = getCurlFunction(libcurl, "curl_easy_init");
    void* easy_setopt_addr = getCurlFunction(libcurl, "curl_easy_setopt");

    if (!easy_init_addr or !easy_setopt_addr) return log::error(
        "Failed to find curl functions!"
    );

    original_easy_init = (curl_easy_init_t)easy_init_addr;
    original_easy_setopt = (curl_easy_setopt_t)easy_setopt_addr;

    log::info("curl_easy_init at {:p}", easy_init_addr);
    log::info("curl_easy_setopt at {:p}", easy_setopt_addr);

    if (auto err = Mod::get()->hook(
        easy_init_addr,
        &hooked_curl_easy_init,
        "curl_easy_init",
        tulip::hook::TulipConvention::Cdecl
    ).err()) return log::error(
        "Failed to hook: {}", err
    );
}