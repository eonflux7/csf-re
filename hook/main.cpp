// Hook DLL entry. Loaded as a dinput8.dll proxy from the game directory.
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_set>
#include "hook.h"

namespace hook {

static Entry* g_head;
static FILE* g_log;
static char g_dir[MAX_PATH];

void add(Entry* e) {
    e->next = g_head;
    g_head = e;
}

void log(const char* fmt, ...) {
    if (!g_log)
        return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_log, fmt, ap);
    va_end(ap);
    fputc('\n', g_log);
    fflush(g_log);
}

static bool patch(void* at, const void* bytes, size_t n) {
    DWORD old;
    if (!VirtualProtect(at, n, PAGE_EXECUTE_READWRITE, &old))
        return false;
    memcpy(at, bytes, n);
    VirtualProtect(at, n, old, &old);
    FlushInstructionCache(GetCurrentProcess(), at, n);
    return true;
}

static bool install(const Entry& e) {
    if (e.kind == Kind::Jmp) {
        uint8_t jmp[5] = {0xE9};
        int32_t rel = (int32_t)((uintptr_t)e.fn - (e.addr + 5));
        memcpy(jmp + 1, &rel, 4);
        return patch((void*)e.addr, jmp, sizeof jmp);
    }
    void* slot = (void*)(e.addr + 4 * e.slot);
    return patch(slot, &e.fn, sizeof(void*));
}

// csf_hook.ini: one directive per line; '#' comments.
//   disable 0x4A1230        disable one hook
//   disable_module ped      disable all hooks of a module
//   only_module ped         install only hooks of listed modules (repeatable)
struct Config {
    std::unordered_set<uintptr_t> disabled;
    std::unordered_set<std::string> disabled_modules, only_modules;
};

static Config load_config() {
    Config c;
    std::string path = std::string(g_dir) + "csf_hook.ini";
    FILE* f = fopen(path.c_str(), "r");
    if (!f)
        return c;
    char line[512], key[64], val[256];
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || sscanf(line, "%63s %255s", key, val) != 2)
            continue;
        if (!strcmp(key, "disable"))
            c.disabled.insert((uintptr_t)strtoul(val, nullptr, 0));
        else if (!strcmp(key, "disable_module"))
            c.disabled_modules.insert(val);
        else if (!strcmp(key, "only_module"))
            c.only_modules.insert(val);
    }
    fclose(f);
    return c;
}

void install_all() {
    Config cfg = load_config();
    int ok = 0, skipped = 0, failed = 0;
    for (Entry* e = g_head; e; e = e->next) {
        bool off = cfg.disabled.count(e->addr) || cfg.disabled_modules.count(e->module) ||
                   (!cfg.only_modules.empty() && !cfg.only_modules.count(e->module));
        if (off) {
            ++skipped;
            continue;
        }
        if (install(*e)) {
            ++ok;
            log("hook %08x %-8s %s", (unsigned)e->addr, e->module, e->name);
        } else {
            ++failed;
            log("FAILED hook %08x %s", (unsigned)e->addr, e->name);
        }
    }
    log("hooks installed=%d skipped=%d failed=%d", ok, skipped, failed);
}

static void init() {
    GetModuleFileNameA(nullptr, g_dir, MAX_PATH);
    if (char* s = strrchr(g_dir, '\\'))
        s[1] = 0;
    g_log = fopen((std::string(g_dir) + "csf_hook.log").c_str(), "w");
    log("csf hook loaded; exe base %p", GetModuleHandleA(nullptr));
    install_all();
}

}  // namespace hook

// ---- dinput8 proxy -------------------------------------------------------------------------------
static HMODULE g_real;
using DI8Create = HRESULT(WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

extern "C" HRESULT WINAPI DirectInput8Create(HINSTANCE inst, DWORD ver, REFIID riid, LPVOID* out, LPUNKNOWN outer) {
    if (!g_real) {
        char path[MAX_PATH];
        GetSystemDirectoryA(path, MAX_PATH);
        strcat(path, "\\dinput8.dll");
        g_real = LoadLibraryA(path);
    }
    auto fn = g_real ? (DI8Create)GetProcAddress(g_real, "DirectInput8Create") : nullptr;
    return fn ? fn(inst, ver, riid, out, outer) : E_FAIL;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        hook::init();
    return TRUE;
}
