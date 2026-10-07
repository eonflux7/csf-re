// Autopilot: scripted input for unattended smoke tests (skip intros, load a save, play a while).
//
// csf_hook.ini directives (time = ms since the DLL loaded):
//   autopilot 9000 key ESCAPE          press+release a key (DIK name or 0xNN scan code), 120 ms hold
//   autopilot 30000 hold 2000 UP       hold a key for 2000 ms
//
// Injection paths, both used for every step: the game's DirectInput keyboard (GetDeviceState / GetDeviceData
// are wrapped by patching the device vtable after CreateDevice) and WM_KEYDOWN/WM_KEYUP posted to the process'
// top-level windows (for UI code that reads window messages).
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>

namespace hook {
void log(const char* fmt, ...);
extern char g_dir[MAX_PATH];
}

namespace {

struct Step {
    DWORD at, hold;
    uint8_t dik;
    bool pressed = false, released = false;
};

std::vector<Step> g_steps;
std::mutex g_mu;
DWORD g_t0;
struct Event { uint8_t dik; bool down; DWORD time; };
std::vector<Event> g_pending;  // for buffered GetDeviceData
DWORD g_seq = 0x10000000;

struct { const char* name; uint8_t dik; } kKeys[] = {
    {"ESCAPE", 0x01}, {"RETURN", 0x1C}, {"ENTER", 0x1C}, {"SPACE", 0x39}, {"TAB", 0x0F}, {"UP", 0xC8},
    {"DOWN", 0xD0}, {"LEFT", 0xCB}, {"RIGHT", 0xCD}, {"W", 0x11}, {"A", 0x1E}, {"S", 0x1F}, {"D", 0x20},
    {"F1", 0x3B}, {"F5", 0x3F}, {"F9", 0x43}, {"LSHIFT", 0x2A}, {"LCONTROL", 0x1D},
};

uint8_t parse_key(const char* s) {
    for (auto& k : kKeys)
        if (!_stricmp(k.name, s))
            return k.dik;
    return (uint8_t)strtoul(s, nullptr, 0);
}

bool key_down_now(uint8_t dik, DWORD now) {
    for (auto& s : g_steps)
        if (s.dik == dik && now >= s.at && now < s.at + s.hold)
            return true;
    return false;
}

BOOL CALLBACK post_key(HWND w, LPARAM lp) {
    DWORD pid;
    GetWindowThreadProcessId(w, &pid);
    if (pid == GetCurrentProcessId() && IsWindowVisible(w)) {
        auto* e = (Event*)lp;
        UINT vk = MapVirtualKeyA(e->dik & 0x7F, 0 /*MAPVK_VSC_TO_VK*/);
        if (e->dik == 0xC8) vk = VK_UP;
        if (e->dik == 0xD0) vk = VK_DOWN;
        if (e->dik == 0xCB) vk = VK_LEFT;
        if (e->dik == 0xCD) vk = VK_RIGHT;
        LPARAM l = 1 | ((LPARAM)(e->dik & 0x7F) << 16) | (e->down ? 0 : (3u << 30));
        PostMessageA(w, e->down ? WM_KEYDOWN : WM_KEYUP, vk, l);
    }
    return TRUE;
}

DWORD WINAPI driver(void*) {
    for (;;) {
        Sleep(10);
        DWORD now = GetTickCount() - g_t0;
        std::lock_guard<std::mutex> lk(g_mu);
        bool left = false;
        for (auto& s : g_steps) {
            if (!s.pressed && now >= s.at) {
                s.pressed = true;
                Event e{s.dik, true, GetTickCount()};
                g_pending.push_back(e);
                EnumWindows(post_key, (LPARAM)&e);
                hook::log("autopilot t=%lu key 0x%02X down", now, s.dik);
            }
            if (s.pressed && !s.released && now >= s.at + s.hold) {
                s.released = true;
                Event e{s.dik, false, GetTickCount()};
                g_pending.push_back(e);
                EnumWindows(post_key, (LPARAM)&e);
            }
            left |= !s.released;
        }
        if (!left)
            return 0;
    }
}

// ---- DirectInput vtable patches ----
const GUID kSysKeyboard = {0x6F1D2B61, 0xD5A0, 0x11CF, {0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
using GetStateFn = HRESULT(__stdcall*)(void*, DWORD, void*);
using GetDataFn = HRESULT(__stdcall*)(void*, DWORD, void*, DWORD*, DWORD);
using CreateDevFn = HRESULT(__stdcall*)(void*, const GUID&, void**, void*);
GetStateFn g_getState;
GetDataFn g_getData;
CreateDevFn g_createDev;
void* g_keyboard;

struct DIOD { DWORD dwOfs, dwData, dwTimeStamp, dwSequence; UINT_PTR uAppData; };  // DIDEVICEOBJECTDATA (DI8)

HRESULT __stdcall GetDeviceState_(void* self, DWORD cb, void* data) {
    HRESULT hr = g_getState(self, cb, data);
    if (self == g_keyboard && cb >= 256 && data) {
        DWORD now = GetTickCount() - g_t0;
        std::lock_guard<std::mutex> lk(g_mu);
        for (auto& s : g_steps)
            if (key_down_now(s.dik, now))
                ((uint8_t*)data)[s.dik] = 0x80;
        if (FAILED(hr))
            hr = 0;  // not acquired (no focus): still deliver the scripted keys
    }
    return hr;
}

HRESULT __stdcall GetDeviceData_(void* self, DWORD cb, void* rg, DWORD* inout, DWORD flags) {
    DWORD room = inout ? *inout : 0;  // caller's capacity (overwritten with the count by the call)
    HRESULT hr = g_getData(self, cb, rg, inout, flags);
    if (self == g_keyboard && rg && inout && cb >= sizeof(DIOD) - sizeof(UINT_PTR) && !(flags & 1 /*PEEK*/)) {
        std::lock_guard<std::mutex> lk(g_mu);
        DWORD n = FAILED(hr) ? 0 : *inout;
        size_t used = 0;
        for (; used < g_pending.size() && n < room; ++used, ++n) {
            DIOD* d = (DIOD*)((uint8_t*)rg + n * cb);
            memset(d, 0, cb);
            d->dwOfs = g_pending[used].dik;
            d->dwData = g_pending[used].down ? 0x80 : 0;
            d->dwTimeStamp = g_pending[used].time;
            d->dwSequence = g_seq++;
        }
        g_pending.erase(g_pending.begin(), g_pending.begin() + used);
        *inout = n;
        if (FAILED(hr) && n)
            hr = 0;
    }
    return hr;
}

void patch(void** vtbl, int slot, void* fn, void** orig) {
    DWORD old;
    VirtualProtect(&vtbl[slot], sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
    if (vtbl[slot] != fn) {
        *orig = vtbl[slot];
        vtbl[slot] = fn;
    }
    VirtualProtect(&vtbl[slot], sizeof(void*), old, &old);
}

HRESULT __stdcall CreateDevice_(void* self, const GUID& guid, void** dev, void* outer) {
    HRESULT hr = g_createDev(self, guid, dev, outer);
    if (SUCCEEDED(hr) && dev && *dev && !memcmp(&guid, &kSysKeyboard, sizeof(GUID))) {
        g_keyboard = *dev;
        void** vt = *(void***)*dev;
        patch(vt, 9, (void*)&GetDeviceState_, (void**)&g_getState);
        patch(vt, 10, (void*)&GetDeviceData_, (void**)&g_getData);
        hook::log("autopilot: keyboard device %p wrapped", *dev);
    }
    return hr;
}

}  // namespace

namespace hook {

void autopilot_init() {
    g_t0 = GetTickCount();
    FILE* f = fopen((std::string(g_dir) + "csf_hook.ini").c_str(), "r");
    if (!f)
        return;
    char line[256], a[32], b[32], c[32], d[32], e[32];
    while (fgets(line, sizeof line, f)) {
        int n = sscanf(line, "%31s %31s %31s %31s %31s", a, b, c, d, e);
        if (n >= 4 && !strcmp(a, "autopilot")) {
            Step s{(DWORD)strtoul(b, nullptr, 0), 120, 0};
            if (!strcmp(c, "key"))
                s.dik = parse_key(d);
            else if (!strcmp(c, "hold") && n >= 5)
                s.hold = (DWORD)strtoul(d, nullptr, 0), s.dik = parse_key(e);
            if (s.dik)
                g_steps.push_back(s);
        }
    }
    fclose(f);
    if (!g_steps.empty()) {
        log("autopilot: %u steps", (unsigned)g_steps.size());
        CreateThread(nullptr, 0, driver, nullptr, 0, nullptr);
    }
}

const char* ini_string(const char* key) {
    static char buf[16][512];
    static int used;
    FILE* f = fopen((std::string(g_dir) + "csf_hook.ini").c_str(), "r");
    if (!f)
        return nullptr;
    char line[512];
    size_t kl = strlen(key);
    const char* out = nullptr;
    while (!out && fgets(line, sizeof line, f)) {
        if (!strncmp(line, key, kl) && (line[kl] == ' ' || line[kl] == '\t') && used < 16) {
            char* v = line + kl;
            while (*v == ' ' || *v == '\t')
                ++v;
            size_t n = strcspn(v, "\r\n");
            v[n] = 0;
            out = strcpy(buf[used++], v);
        }
    }
    fclose(f);
    return out;
}

void autopilot_wrap_dinput(void* di) {
    if (g_steps.empty() || !di)
        return;
    patch(*(void***)di, 3, (void*)&CreateDevice_, (void**)&g_createDev);
}

}  // namespace hook
