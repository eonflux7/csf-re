// Original game behavior for slots whose DLL hooks add developer/test options.
// Only csf_standalone_startup compiles this file; the DLL uses core/devcmd.cpp.
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cstring>
#include <cstdio>
#include <initializer_list>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, String_Assign, 0x429B60,
         (void* self, void* edx, const char* text, unsigned int length));
CSF_FUNC(char, __fastcall, DevCommandLine_Parse, 0x4FB850, (void* self, void* edx));
CSF_FUNC(uint8_t*, __cdecl, App_Get, 0x50D190, ());
CSF_FUNC(uint8_t*, __cdecl, Render_GetSystem, 0x6B1380, ());
CSF_FUNC(void*, __fastcall, Render_GetDevice, 0x652160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790,
         (void* self, void* edx, int entry, int argument, int request, int configuration));
CSF_GLOBAL(void*, g_MenuCallbackManager, 0x008B0570);

CSF_FUNC(int, __fastcall, CSoundManager_ReleasePoolsAndReset, 0x005238B0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, SoundManager_Free, 0x00691110, (void* block));

// Scalar deleting destructor and its two secondary-base adjustors. IDA missed
// all three entries; 0084106C/00841064 hold the +4/+8-base vtables.
static void* __fastcall CSoundManager_ScalarDelete(void* self, void*, unsigned flags) {
    CSoundManager_ReleasePoolsAndReset(self, nullptr);
    if (flags & 1) SoundManager_Free(self);
    return self;
}
HOOK(0x005240F0, CSoundManager_ScalarDelete, "auto");

static void* const soundManagerDelete = reinterpret_cast<void*>(CSF_ADDR_FN(0x005240F0));
static __declspec(naked) void CSoundManager_DeleteFromBase4() {
    __asm sub ecx, 4
    __asm jmp dword ptr [soundManagerDelete]
}
static __declspec(naked) void CSoundManager_DeleteFromBase8() {
    __asm sub ecx, 8
    __asm jmp dword ptr [soundManagerDelete]
}
HOOK(0x00523920, CSoundManager_DeleteFromBase4, "auto");
HOOK(0x00523930, CSoundManager_DeleteFromBase8, "auto");

// Independently callable cdecl callback (0050F920..0050F972), missed by IDA.
// 0050FBD0 registers it through 006973B0, rather than calling it directly.
static int __cdecl GameBddLoader_LoadCompleteCallback() {
    void* manager = g_MenuCallbackManager;
    void* box = CSF_AT(void*, manager, 0xF0);
    if (box) {
        (reinterpret_cast<void (__thiscall*)(void*)>(CSF_AT(void**, box, 0)[0x40 / 4]))(box);
        box = CSF_AT(void*, manager, 0xF0);
        CSF_AT(uint32_t, manager, 0xF4) = CSF_AT(uint32_t, box, 0x78);
        CSF_AT(void*, manager, 0xF0) = nullptr;
        manager = g_MenuCallbackManager;
    }
    return GameBddLoader_DispatchMenuRequest(manager, nullptr, 0x47323037, 8, 0x1800, 0);
}
HOOK(0x0050F920, GameBddLoader_LoadCompleteCallback, "auto");

using AdjustWindowRectFn = BOOL (__stdcall*)(RECT*, DWORD, BOOL);
using GetSystemMetricsFn = int (__stdcall*)(int);
using MoveWindowFn = BOOL (__stdcall*)(HWND, int, int, int, int, BOOL);
CSF_GLOBAL(AdjustWindowRectFn, g_AdjustWindowRect, 0x82D2C0);
CSF_GLOBAL(GetSystemMetricsFn, g_GetSystemMetrics, 0x82D2BC);
CSF_GLOBAL(MoveWindowFn, g_MoveWindow, 0x82D2B8);

// Store the configuration command line in the string at +0, then parse it.
static char __fastcall DevCommandLine_Set(void* self, void*, const char* text) {
    String_Assign(self, nullptr, text, static_cast<unsigned int>(std::strlen(text)));
    return DevCommandLine_Parse(self, nullptr);
}

// Nonpositive dimensions independently fall back to the render device's size.
static BOOL __cdecl Window_FitClientArea(HWND window, LONG width, LONG height) {
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    if (width <= 0) {
        void* device = Render_GetDevice(Render_GetSystem(), nullptr);
        width = (reinterpret_cast<LONG (__thiscall*)(void*)>(
            CSF_AT(void**, device, 0)[8]))(device);
    }
    rect.right = width;
    if (height <= 0) {
        void* device = Render_GetDevice(Render_GetSystem(), nullptr);
        height = (reinterpret_cast<LONG (__thiscall*)(void*)>(
            CSF_AT(void**, device, 0)[9]))(device);
    }
    rect.bottom = height;
    g_AdjustWindowRect(&rect, 0xCA0000, FALSE);
    const int outerWidth = rect.right - rect.left;
    const int outerHeight = rect.bottom - rect.top;
    return g_MoveWindow(window, (g_GetSystemMetrics(SM_CXSCREEN) - outerWidth) / 2,
                        0, outerWidth, outerHeight, TRUE);
}

// The panel, application flag and renderer state gate the mission briefing.
static BOOL __fastcall GameScreen_HasBriefing(uint8_t* self, void*) {
    return CSF_AT(uint32_t, self, 0x138) &&
           CSF_AT(uint8_t, CSF_AT(uint8_t*, App_Get(), 0x1C), 0x50) &&
           !CSF_AT(uint8_t, Render_GetSystem(), 4);
}

// Scripted input for unattended runs (tools/dryrun.py --input): CSF_INPUT="T:key:DIK;T:goto:X,Y;T:click;T:rclick;T:wheel:N"
// with T in seconds since the first DirectInput device was created. Windows' synthetic input does not reach the game's
// exclusive DirectInput devices, so events are appended to what GetDeviceData returns (both devices are buffered,
// 0x713400). The cursor moves by relative deltas: goto parks it at (0,0) with a large negative move, then moves to
// (X,Y). Each injected step prints "csf: input ...".
namespace {
struct ScriptStep { DWORD at; int kind; int a, b; };  // kind: 0 key, 1 goto, 2 click, 3 rclick, 4 wheel
// Fixed storage: no dynamic initialisers or atexit destructors in this file (tests/audio1_offline.py and
// tests/wmrest_offline.py run its initialisers in Unicorn).
struct EventQueue {
    DIDEVICEOBJECTDATA items[256];
    size_t head, tail;
    bool empty() const { return head == tail; }
    const DIDEVICEOBJECTDATA& front() const { return items[head % 256]; }
    void pop() { ++head; }
    void push(const DIDEVICEOBJECTDATA& e) { if (tail - head < 256) items[tail++ % 256] = e; }
};
ScriptStep g_script[128];
size_t g_scriptCount, g_scriptNext;
DWORD g_scriptStart;
EventQueue g_pendingKeys, g_pendingMouse;
using GetDeviceDataFn = HRESULT (STDMETHODCALLTYPE*)(void*, DWORD, DIDEVICEOBJECTDATA*, DWORD*, DWORD);
using CreateDeviceFn = HRESULT (STDMETHODCALLTYPE*)(void*, REFGUID, void**, void*);
CreateDeviceFn g_createDevice;

void ScriptedInput_Load() {
    char text[4096];
    if (!GetEnvironmentVariableA("CSF_INPUT", text, sizeof text)) return;
    for (char* step = strtok(text, ";"); step; step = strtok(nullptr, ";")) {
        double t = 0; char kind[16] = {}; int a = 0, b = 0;
        if (sscanf(step, "%lf:%15[a-z]:%d,%d", &t, kind, &a, &b) < 2) continue;
        const int k = !strcmp(kind, "key") ? 0 : !strcmp(kind, "goto") ? 1 : !strcmp(kind, "click") ? 2
                    : !strcmp(kind, "rclick") ? 3 : !strcmp(kind, "wheel") ? 4 : -1;
        if (k >= 0 && g_scriptCount < 128) g_script[g_scriptCount++] = {static_cast<DWORD>(t * 1000), k, a, b};
    }
}
void Push(EventQueue& q, DWORD offset, DWORD data) {
    DIDEVICEOBJECTDATA e{}; e.dwOfs = offset; e.dwData = data; e.dwTimeStamp = GetTickCount(); q.push(e);
}
void ScriptedInput_Advance() {
    const DWORD now = GetTickCount() - g_scriptStart;
    while (g_scriptNext < g_scriptCount && g_script[g_scriptNext].at <= now) {
        const ScriptStep& s = g_script[g_scriptNext++];
        std::fprintf(stderr, "csf: input t=%.1f kind=%d %d,%d\n", now / 1000.0, s.kind, s.a, s.b);
        switch (s.kind) {
        case 0: Push(g_pendingKeys, s.a, 0x80); Push(g_pendingKeys, s.a, 0); break;
        case 1: Push(g_pendingMouse, DIMOFS_X, static_cast<DWORD>(-20000)); Push(g_pendingMouse, DIMOFS_Y, static_cast<DWORD>(-20000));
                Push(g_pendingMouse, DIMOFS_X, static_cast<DWORD>(s.a)); Push(g_pendingMouse, DIMOFS_Y, static_cast<DWORD>(s.b)); break;
        case 2: Push(g_pendingMouse, DIMOFS_BUTTON0, 0x80); Push(g_pendingMouse, DIMOFS_BUTTON0, 0); break;
        case 3: Push(g_pendingMouse, DIMOFS_BUTTON1, 0x80); Push(g_pendingMouse, DIMOFS_BUTTON1, 0); break;
        case 4: Push(g_pendingMouse, DIMOFS_Z, static_cast<DWORD>(s.a)); break;
        }
    }
}
// Appends pending events (down and up of one press go out in separate polls, so the game sees a press).
HRESULT Inject(GetDeviceDataFn real, EventQueue& q, void* self, DWORD size, DIDEVICEOBJECTDATA* data,
               DWORD* count, DWORD flags) {
    const DWORD capacity = count ? *count : 0;
    HRESULT result = real(self, size, data, count, flags);
    ScriptedInput_Advance();
    if (q.empty() || !data || !count || size != sizeof(DIDEVICEOBJECTDATA) || (flags & DIGDD_PEEK)) return result;
    if (FAILED(result)) { *count = 0; result = DI_OK; }
    DWORD added = 0;
    bool sawButtonDown = false;
    while (!q.empty() && *count < capacity) {
        const DIDEVICEOBJECTDATA& e = q.front();
        const bool button = e.dwOfs >= DIMOFS_BUTTON0 && e.dwOfs <= DIMOFS_BUTTON7;
        if ((button || &q == &g_pendingKeys) && !e.dwData && sawButtonDown) break;  // release in the next poll
        if (e.dwData == 0x80) sawButtonDown = true;
        data[(*count)++] = e; q.pop(); ++added;
    }
    return result;
}
void* g_keyboard;
void* g_mouse;
GetDeviceDataFn g_getDeviceData;
HRESULT STDMETHODCALLTYPE DeviceData(void* self, DWORD size, DIDEVICEOBJECTDATA* data, DWORD* count, DWORD flags) {
    if (self == g_keyboard) return Inject(g_getDeviceData, g_pendingKeys, self, size, data, count, flags);
    if (self == g_mouse) return Inject(g_getDeviceData, g_pendingMouse, self, size, data, count, flags);
    return g_getDeviceData(self, size, data, count, flags);
}
// dinput8 tells its A and W interfaces apart by the object's vtable pointer, so the shared vtable's slot is replaced
// in place (VirtualProtect), not the object's vtable pointer.
void PatchSlot(void* object, int slot, void* fn, void** original) {
    void** table = *static_cast<void***>(object);
    if (table[slot] == fn) return;
    DWORD old;
    if (!VirtualProtect(&table[slot], sizeof(void*), PAGE_READWRITE, &old)) return;
    *original = table[slot];
    table[slot] = fn;
    VirtualProtect(&table[slot], sizeof(void*), old, &old);
}
HRESULT STDMETHODCALLTYPE CreateDevice(void* self, REFGUID guid, void** device, void* outer) {
    const HRESULT result = g_createDevice(self, guid, device, outer);
    if (SUCCEEDED(result) && device && *device) {
        if (!g_scriptStart) g_scriptStart = GetTickCount();
        if (guid == GUID_SysKeyboard) g_keyboard = *device;
        if (guid == GUID_SysMouse) g_mouse = *device;
        if (!g_getDeviceData) PatchSlot(*device, 10, reinterpret_cast<void*>(&DeviceData), reinterpret_cast<void**>(&g_getDeviceData));
    }
    return result;
}
}  // namespace
static void ScriptedInput_WrapInterface(void* directInput) {
    ScriptedInput_Load();
    if (g_scriptCount && !g_createDevice)
        PatchSlot(directInput, 3, reinterpret_cast<void*>(&CreateDevice), reinterpret_cast<void**>(&g_createDevice));
}

// 0x7DB650 is the exe's jump through the DINPUT8 import; the standalone loads System32's dinput8.dll itself.
// The exe's c_dfDIKeyboard (0x883BD4) and c_dfDIMouse (0x8839CC) come from the static dinput8.lib and point at object
// arrays it placed in .text (0x7DA650, 0x7DA5E0), which the standalone does not have: the SDK's dinput8.lib copies
// replace them before the game's first SetDataFormat.
static HRESULT WINAPI Import_DirectInput8Create(HINSTANCE inst, DWORD ver, const void* riid, void** out,
                                                void* outer) {
    using Create = HRESULT (WINAPI*)(HINSTANCE, DWORD, const void*, void**, void*);
    static Create real;
    if (!real) {
        std::memcpy(reinterpret_cast<void*>(CSF_ADDR_DATA(0x00883BD4)), &c_dfDIKeyboard, sizeof(DIDATAFORMAT));
        std::memcpy(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008839CC)), &c_dfDIMouse, sizeof(DIDATAFORMAT));
        char path[MAX_PATH];
        GetSystemDirectoryA(path, MAX_PATH);
        strcat(path, "\\dinput8.dll");
        if (HMODULE m = LoadLibraryA(path))
            real = reinterpret_cast<Create>(GetProcAddress(m, "DirectInput8Create"));
    }
    if (!real) return E_FAIL;
    const HRESULT result = real(inst, ver, riid, out, outer);
    if (SUCCEEDED(result) && out && *out) ScriptedInput_WrapInterface(*out);
    return result;
}

HOOK(0x4FB8F0, DevCommandLine_Set, "auto");
HOOK(0x508F50, Window_FitClientArea, "auto");
HOOK(0x5364D0, GameScreen_HasBriefing, "auto");
HOOK(0x7DB650, Import_DirectInput8Create, "boundary-import");

// Destructor of the class with vtables 0x830578/+4 0x830528/+0x10 0x830520 (Settings -> Game), missed by IDA;
// called from 0x5371EB. Frees the +0xE4 block, then unwinds to the 0x83042C base.
static void __fastcall C_00830578_Destroy(void* self, void*) {
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00830578);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x00830528);
    CSF_AT(uint32_t, self, 0x10) = CSF_ADDR_VT(0x00830520);
    if (void* block = CSF_AT(void*, self, 0xE4)) {
        SoundManager_Free(block);
        CSF_AT(void*, self, 0xE4) = nullptr;
    }
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_DATA(0x00830488);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x00830438);
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x0083042C);
}
HOOK(0x0042B320, C_00830578_Destroy, "auto");

// Code IDA left outside any function: reached only through vtable slots / adjustor thunks (no function at all),
// or kept as another function's chunk while our sources call it as a function (tools/gen_adjustors.py covers the
// thunks themselves). Without the original bytes these slots trapped in the standalone.
CSF_FUNC(void, __fastcall, C_008311c4_Destroy, 0x0044CC90, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Destroy_454F10, 0x00454F10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Destroy_457090, 0x00457090, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Destroy_46B340, 0x0046B340, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Destroy_520BD0, 0x00520BD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Scene_ResetTrackedObjects, 0x0044D3F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00860a30_InitBase, 0x006CA660, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S00402350_InitTransform, 0x00402350, (void* self, void* edx));

// Scalar deleting destructors: destroy, then free when flags & 1.
#define CSF_SCALAR_DELETE(addr, name, destroy)                     \
    static void* __fastcall name(void* self, void*, unsigned flags) { \
        destroy(self, nullptr);                                     \
        if (flags & 1) SoundManager_Free(self);                     \
        return self;                                                \
    }                                                               \
    HOOK(addr, name, "auto");
CSF_SCALAR_DELETE(0x0044CD80, C_008311c4_ScalarDelete, C_008311c4_Destroy)
CSF_SCALAR_DELETE(0x00455D80, CScene_ScalarDelete, Destroy_454F10)
CSF_SCALAR_DELETE(0x00457120, C_008312e0_ScalarDelete, Destroy_457090)
CSF_SCALAR_DELETE(0x0046BC10, CMissionScript_ScalarDelete, Destroy_46B340)
CSF_SCALAR_DELETE(0x00520DF0, C_00840fdc_ScalarDelete, Destroy_520BD0)

// Scalar deleting destructor of the class with vtables 0x8311E8/+4 0x8311CC.
static void* __fastcall C_008311e8_ScalarDelete(void* self, void*, unsigned flags) {
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x008311E8);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x008311CC);
    Scene_ResetTrackedObjects(self, nullptr);
    if (void* block = CSF_AT(void*, self, 0x08)) {
        SoundManager_Free(block);
        CSF_AT(void*, self, 0x08) = nullptr;
    }
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x0082D410);
    if (flags & 1) SoundManager_Free(self);
    return self;
}
HOOK(0x0044D550, C_008311e8_ScalarDelete, "auto");

// Chunk of 0x742C80: slot 2 of the object in ECX with argument 0.
static int __fastcall CallSlot2WithZero(void* self, void*) {
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[2])(self, nullptr, 0);
}
HOOK(0x00661DC0, CallSlot2WithZero, "auto");

// Chunks of 0x6C9BE0: two constructors on the 0x6CA660 base.
static void* __fastcall C_00861508_Construct(void* self, void*) {
    C_00860a30_InitBase(self, nullptr);
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00861508);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x00861500);
    CSF_AT(uint32_t, self, 0x08) = CSF_ADDR_VT(0x008614E4);
    S00402350_InitTransform(static_cast<uint8_t*>(self) + 0x20, nullptr);
    for (int o : {0x5C, 0x60, 0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x7C, 0x88, 0x8C, 0x90, 0x94})
        CSF_AT(uint32_t, self, o) = 0;
    return self;
}
HOOK(0x006DCA90, C_00861508_Construct, "auto");
static void* __fastcall C_00862390_Construct(void* self, void*) {
    C_00860a30_InitBase(self, nullptr);
    for (int o : {0x20, 0x24, 0x28, 0x2C, 0x34, 0x38, 0x30})
        CSF_AT(uint32_t, self, o) = 0;
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00862390);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x00862384);
    CSF_AT(uint32_t, self, 0x08) = CSF_ADDR_VT(0x00862368);
    return self;
}
HOOK(0x006F0CB0, C_00862390_Construct, "auto");
