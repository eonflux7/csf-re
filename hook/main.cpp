// Hook DLL entry. Loaded as a dinput8.dll proxy from the game directory.
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_set>
#include <vector>
#include "hook.h"
#include "MinHook.h"

namespace hook {

static Entry* g_head;
static FILE* g_log;
char g_dir[MAX_PATH];  // exe directory (shared with autopilot.cpp)

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

// Per-hook hit counters: each hook goes through a stub `inc dword [count]; jmp fn`, dumped to
// csf_hook_hits.txt every few seconds (coverage of a playtest). 16 bytes per stub.
static uint8_t* g_stubs;
static uint32_t* g_hits;
static Entry** g_installed;
static int g_ninstalled;

static void* make_stub(int i, void* fn) {
    uint8_t* s = g_stubs + 16 * i;
    s[0] = 0xFF; s[1] = 0x05;  // inc dword ptr [abs32]
    uint32_t* cnt = &g_hits[i];
    memcpy(s + 2, &cnt, 4);
    s[6] = 0xE9;               // jmp rel32
    int32_t rel = (int32_t)((uintptr_t)fn - (uintptr_t)(s + 11));
    memcpy(s + 7, &rel, 4);
    return s;
}

static bool install(const Entry& e, void* target) {
    if (e.kind == Kind::Tramp)  // MinHook relocates the prologue; *orig calls the original
        return MH_CreateHook((void*)e.addr, target, e.orig) == MH_OK && MH_EnableHook((void*)e.addr) == MH_OK;
    if (e.kind == Kind::Jmp) {
        uint8_t jmp[5] = {0xE9};
        int32_t rel = (int32_t)((uintptr_t)target - (e.addr + 5));
        memcpy(jmp + 1, &rel, 4);
        return patch((void*)e.addr, jmp, sizeof jmp);
    }
    void* slot = (void*)(e.addr + 4 * e.slot);
    return patch(slot, &target, sizeof(void*));
}

static DWORD WINAPI hits_thread(LPVOID) {
    std::string path = std::string(g_dir) + "csf_hook_hits.txt";
    for (;;) {
        Sleep(3000);
        if (FILE* f = fopen(path.c_str(), "w")) {
            for (int i = 0; i < g_ninstalled; ++i)
                fprintf(f, "%08x %10u %s %s\n", (unsigned)g_installed[i]->addr, g_hits[i],
                        g_installed[i]->module, g_installed[i]->name);
            fclose(f);
        }
    }
}

// csf_hook.ini: one directive per line; '#' comments.
//   disable 0x4A1230        disable one hook
//   disable_module ped      disable all hooks of a module
//   only_module ped         install only hooks of listed modules (repeatable)
//   probe 0x538160          count calls of an original function (pass-through; shows in csf_hook_hits.txt)
struct Config {
    std::unordered_set<uintptr_t> disabled;
    std::vector<uintptr_t> probes;
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
        else if (!strcmp(key, "probe"))
            c.probes.push_back((uintptr_t)strtoul(val, nullptr, 0));
    }
    fclose(f);
    return c;
}

void install_all() {
    Config cfg = load_config();
    int ok = 0, skipped = 0, failed = 0, n = 0;
    MH_Initialize();
    for (Entry* e = g_head; e; e = e->next)
        ++n;
    n += (int)cfg.probes.size();
    g_stubs = (uint8_t*)VirtualAlloc(nullptr, 16 * n + 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    g_hits = new uint32_t[n]();
    g_installed = new Entry*[n];
    for (Entry* e = g_head; e; e = e->next) {
        bool off = cfg.disabled.count(e->addr) || cfg.disabled_modules.count(e->module) ||
                   (!cfg.only_modules.empty() && !cfg.only_modules.count(e->module));
        if (off) {
            ++skipped;
            continue;
        }
        if (install(*e, make_stub(g_ninstalled, e->fn))) {
            g_installed[g_ninstalled++] = e;
            ++ok;
            log("hook %08x %-8s %s", (unsigned)e->addr, e->module, e->name);
        } else {
            ++failed;
            log("FAILED hook %08x %s", (unsigned)e->addr, e->name);
        }
    }
    for (uintptr_t a : cfg.probes) {  // counting stub -> MinHook trampoline (the original, relocated)
        auto* e = new Entry{a, 0, nullptr, "probe", "probe", Kind::Tramp, nullptr, nullptr};
        uint8_t* stub = (uint8_t*)make_stub(g_ninstalled, nullptr);
        void* orig = nullptr;
        if (MH_CreateHook((void*)a, stub, &orig) == MH_OK) {
            int32_t rel = (int32_t)((uintptr_t)orig - (uintptr_t)(stub + 11));
            memcpy(stub + 7, &rel, 4);
            MH_EnableHook((void*)a);
            g_installed[g_ninstalled++] = e;
            log("probe %08x", (unsigned)a);
        } else {
            log("FAILED probe %08x", (unsigned)a);
        }
    }
    log("hooks installed=%d skipped=%d failed=%d", ok, skipped, failed);
    CreateThread(nullptr, 0, hits_thread, nullptr, 0, nullptr);
}

void autopilot_init();
void autopilot_wrap_dinput(void* di);

static void init() {
    GetModuleFileNameA(nullptr, g_dir, MAX_PATH);
    if (char* s = strrchr(g_dir, '\\'))
        s[1] = 0;
    g_log = fopen((std::string(g_dir) + "csf_hook.log").c_str(), "w");
    log("csf hook loaded; exe base %p", GetModuleHandleA(nullptr));
    install_all();
    autopilot_init();
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
    HRESULT hr = fn ? fn(inst, ver, riid, out, outer) : E_FAIL;
    if (SUCCEEDED(hr) && out)
        hook::autopilot_wrap_dinput(*out);
    return hr;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        hook::init();
    return TRUE;
}
