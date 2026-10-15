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

// Scripted input for unattended runs (tools/dryrun.py --input): CSF_INPUT="T:key:DIK;T:goto:X,Y;T:click;T:rclick;T:wheel:N;T:char:C"
// with T in seconds since the first DirectInput device was created. Windows' synthetic input does not reach the game's
// exclusive DirectInput devices, so events are appended to what GetDeviceData returns (both devices are buffered,
// 0x713400). The cursor moves by relative deltas: goto parks it at (0,0) with a large negative move, then moves to
// (X,Y). Text fields read WM_CHAR (0x508D40), not DirectInput: char posts WM_CHAR C (a code, 8 = backspace) to the
// active window of the polling (game) thread. Each injected step prints "csf: input ...".
namespace {
struct ScriptStep { DWORD at; int kind; int a, b; };  // kind: 0 key, 1 goto, 2 click, 3 rclick, 4 wheel, 5 char
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
                    : !strcmp(kind, "rclick") ? 3 : !strcmp(kind, "wheel") ? 4 : !strcmp(kind, "char") ? 5 : -1;
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
        case 5: PostMessageA(GetActiveWindow(), WM_CHAR, static_cast<WPARAM>(s.a), 1); break;
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

// 0x537280: a slot of vtables 0x842040/0x843728 (the in-game menu page, P_Juego) that IDA left outside any function;
// the mission starts through it. It resets the game-side singletons, loads the interface scene, sizes the HUD text to
// the screen and opens the first page. Two calls take a word the original pushed before an argumentless getter:
// 0x4F4A40 (ret 8) the two floats pushed before 0x4F4920, 0x519640 (ret 4) the state's +0x10 pushed before 0x519420.
namespace enter_level {
using Getter = void* (__cdecl*)();
CSF_FUNC(void*, __fastcall, InitializeCheatScreen, 0x0042B380, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ForwardOffset, 0x00536440, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobal_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ResetThirdManager, 0x00523640, (void* self, void* edx));
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (void* object));
CSF_FUNC(void*, __cdecl, GetGlobal_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, Scheduler_BoundContexts, 0x004FE490, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_NotifySceneChange, 0x004570F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, DefaultDestination, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, EnsureSpawnPointRegistered, 0x0044BB10, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LoadInterfazScene, 0x00535E00, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(void, __fastcall, HudStringShow_SetSize, 0x004F4A40, (void* self, void* edx, float width, float height));
CSF_FUNC(void, __fastcall, RefreshCameraPreview, 0x00542510, (void* self, void* edx, int a, int b));
CSF_FUNC(void*, __fastcall, InitializeDialogPanels, 0x00536270, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetSingleton_519550, 0x00519550, ());
CSF_FUNC(void*, __cdecl, GetGlobal_519420, 0x00519420, ());
CSF_FUNC(void, __fastcall, InitializeSearchDialog, 0x00519640, (void* self, void* edx, void* input));
CSF_FUNC(void*, __fastcall, ResolveOpeningVideo, 0x00536E20, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetValue_8B0574, 0x00518980, ());
CSF_FUNC(uint8_t*, __cdecl, GetGlobal_75FDF0, 0x0075FDF0, ());
CSF_GLOBAL(uint32_t, g_screenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_screenHeight, 0x00899BA0);

template <typename Fn> Fn Slot(void* obj, int offset) { return reinterpret_cast<Fn>(CSF_AT(void**, obj, 0)[offset / 4]); }

static void* __fastcall EnterLevel(uint8_t* self, void*, int page, int argument) {
    void* app = App_Get();
    Slot<void(__fastcall*)(void*, void*)>(app, 0x1C)(app, nullptr);
    InitializeCheatScreen(self, nullptr);
    Slot<void(__fastcall*)(void*, void*)>(self, 0x88)(self, nullptr);
    void* scene = CSF_AT(void*, self, 0x13C);
    Slot<void(__fastcall*)(void*, void*)>(scene, 0x16C)(scene, nullptr);
    CSF_AT(int, self, 0x18) = page;
    ForwardOffset(self, nullptr);
    CSF_AT(uint8_t, self, 0x150) = 0;
    ResetThirdManager(GetGlobal_521BB0(), nullptr);
    MpSpawnPoints_RegisterObject(GetGlobal_521BB0());
    // Singletons created (or reset) in this order; their results are not used.
    const uintptr_t getters[] = {CSF_ADDR_FN(0x006C8BC0), CSF_ADDR_FN(0x00457FD0), CSF_ADDR_FN(0x004565D0),
        CSF_ADDR_FN(0x004EA0D0), CSF_ADDR_FN(0x005193B0), CSF_ADDR_FN(0x00519220), CSF_ADDR_FN(0x005189B0),
        CSF_ADDR_FN(0x00401520), CSF_ADDR_FN(0x0044BAC0), CSF_ADDR_FN(0x00436030), CSF_ADDR_FN(0x004FD6E0),
        CSF_ADDR_FN(0x004F4D20), CSF_ADDR_FN(0x005258B0), CSF_ADDR_FN(0x004429D0), CSF_ADDR_FN(0x0051FFE0)};
    for (uintptr_t getter : getters)
        reinterpret_cast<Getter>(getter)();
    SceneLoader_NotifySceneChange(Scheduler_BoundContexts(GetGlobal_8B0568(), nullptr), nullptr);
    EnsureSpawnPointRegistered(DefaultDestination(GetGlobal_8B0568(), nullptr), nullptr);
    LoadInterfazScene(self, nullptr);

    void* device = Render_GetDevice(Render_GetSystem(), nullptr);
    const uint32_t width = CSF_AT(uint32_t, Slot<void*(__fastcall*)(void*, void*)>(device, 0x18)(device, nullptr), 0);
    device = Render_GetDevice(Render_GetSystem(), nullptr);
    const uint32_t height = CSF_AT(uint32_t, Slot<void*(__fastcall*)(void*, void*)>(device, 0x18)(device, nullptr), 4);
    const float heightRatio = static_cast<float>(static_cast<long double>(height) / g_screenHeight);
    const float widthRatio = static_cast<float>(static_cast<long double>(width) / g_screenWidth);
    HudStringShow_SetSize(GetGlobal8B0558(), nullptr, widthRatio, heightRatio);

    RefreshCameraPreview(CSF_AT(void*, self, 0x14C), nullptr, page, argument);
    Slot<void(__fastcall*)(void*, void*)>(self, 0x74)(self, nullptr);
    InitializeDialogPanels(self, nullptr);
    GetSingleton_519550();
    void* input = CSF_AT(void*, App_Get(), 0x10);
    InitializeSearchDialog(GetGlobal_519420(), nullptr, input);
    ResolveOpeningVideo(self, nullptr);
    GetValue_8B0574();
    void* handler = CSF_AT(void*, App_Get(), 0x10);
    Slot<void(__fastcall*)(void*, void*)>(handler, 0x08)(handler, nullptr);
    if (void* panel = CSF_AT(void*, self, 0x138))
        Slot<void(__fastcall*)(void*, void*, int)>(panel, 0xA8)(panel, nullptr, 1);
    CSF_AT(uint8_t, self, 0x180) = 0;
    if (CSF_AT(uint8_t, App_Get(), 0xD0)) {
        CSF_AT(uint8_t, self, 0x274) = CSF_AT(uint8_t, GetGlobal_75FDF0(), 0xCC);
        CSF_AT(uint8_t, GetGlobal_75FDF0(), 0xCC) = 0;
    }
    return CSF_AT(void*, self, 0x13C);
}
}  // namespace enter_level
HOOK(0x00537280, enter_level::EnterLevel, "auto");

// IDA orphan code. 537490 is the secondary interface's leave slot (this is
// four bytes past the page); its tail jump opens the page following the game.
namespace leave_level {
using enter_level::Slot;
using enter_level::Getter;
CSF_FUNC(void, __fastcall, ResetApp, 0x0050D230, (void*, void*));
CSF_FUNC(void, __cdecl, UnregisterSound, 0x004754D0, (void*));
CSF_FUNC(void*, __cdecl, GetSceneState, 0x004FBDE0, ());
CSF_FUNC(void, __fastcall, PrepareSceneState, 0x004FC440, (void*, void*));
CSF_FUNC(void, __fastcall, ResetScene, 0x00503790, (void*, void*));
CSF_FUNC(void, __fastcall, ClearControls, 0x00536370, (void*, void*));
CSF_FUNC(void, __cdecl, ExitCharacterSelect, 0x00542720, ());
CSF_FUNC(void, __fastcall, ResetPortraits, 0x00457000, (void*, void*));
CSF_FUNC(void, __fastcall, ReleaseRegistration, 0x0044C510, (void*, void*));
CSF_FUNC(void*, __fastcall, GetSceneController, 0x00652010, (void*, void*));
CSF_FUNC(void, __fastcall, SelectScenario, 0x00501990, (void*, void*, char*, char));
CSF_FUNC(void*, __cdecl, GetTextDatabase, 0x0069B700, ());
CSF_FUNC(void, __fastcall, SetTextPath, 0x0069BD90, (void*, void*, unsigned, const char*));
CSF_FUNC(void*, __cdecl, GetConfig, 0x00756A10, ());
CSF_FUNC(int, __fastcall, IsReady, 0x0050C880, (void*, void*));
CSF_FUNC(void, __fastcall, InitPageArgs, 0x0050C860, (void*, void*));
CSF_FUNC(void, __fastcall, SetIntroData, 0x006F2850, (void*, void*, void*));
CSF_FUNC(void, __fastcall, SetCharacterStatus, 0x00542750, (void*, void*, void*));
// Resolve these at use, not through new CRT binding initializers.
static void* LoadingPage() { return CSF_AT(void*, CSF_ADDR_DATA(0x00899B14), 0); }
static void* ReadyPage() { return CSF_AT(void*, CSF_ADDR_DATA(0x00899AE8), 0); }
static void* IntroPage() { return CSF_AT(void*, CSF_ADDR_DATA(0x00899B2C), 0); }
static void* ReturnPage() { return CSF_AT(void*, CSF_ADDR_DATA(0x00899AD0), 0); }
static void* SpecialPage() { return CSF_AT(void*, CSF_ADDR_DATA(0x00899B0C), 0); }
static void* ConfirmPage() { return CSF_AT(void*, CSF_ADDR_DATA(0x00899B98), 0); }
static void* DefaultPage() { return CSF_AT(void*, CSF_ADDR_DATA(0x00899AD8), 0); }

static void* Find(void* menu, void* name) {
    return Slot<void*(__fastcall*)(void*, void*, void*)>(menu, 0x5C)(menu, nullptr, name);
}
static void Show(void* menu, void* name) {
    Slot<void(__fastcall*)(void*, void*, void*)>(menu, 0x58)(menu, nullptr, name);
}
static void __fastcall OpenNextPage(uint8_t* self, void*) {
    void* menu = CSF_AT(void*, self, 0x18);
    void* page = Find(menu, LoadingPage());
    if (Slot<uint8_t(__fastcall*)(void*, void*, void*, int)>(menu, 0x74)(menu, nullptr, page, 0))
        Slot<void(__fastcall*)(void*, void*, void*, int)>(menu, 0x60)(menu, nullptr, page, 0);
    if (CSF_AT(uint8_t, App_Get(), 0xD0)) {
        if (void* select = CSF_AT(void*, self, 0x14C))
            SetCharacterStatus(select, nullptr, CSF_AT(void*, self, 0x18));
        return;
    }
    if (CSF_AT(int, enter_level::GetGlobal_8B0568(), 0x5C) == 1 ||
        CSF_AT(uint8_t, enter_level::GetGlobal_8B0568(), 0xB4)) return;
    if (CSF_AT(uint8_t, GetConfig(), 0xA6C)) {
        Show(CSF_AT(void*, self, 0x18), ReadyPage());
        return;
    }
    if (void* ready = CSF_AT(void*, App_Get(), 0x6C); ready && IsReady(ready, nullptr)) {
        void* args = self + 0x100;
        InitPageArgs(args, nullptr);
        uint8_t* config = static_cast<uint8_t*>(GetConfig());
        void* text = CSF_AT(uint32_t, config, 0x21D0) < 16
            ? config + 0x21BC : CSF_AT(void*, config, 0x21BC);
        CSF_AT(void*, args, 0) = text;
        CSF_AT(int, self, 0x104) = 1;
        CSF_AT(uint8_t, self, 0x112) = 1;
        // args is pushed before Find, but Find ret 4 consumes only the name.
        SetIntroData(Find(CSF_AT(void*, self, 0x18), IntroPage()), nullptr, args);
        Show(CSF_AT(void*, self, 0x18), IntroPage());
    } else if (CSF_AT(uint8_t, self, 0x187)) {
        Show(CSF_AT(void*, self, 0x18), ReturnPage());
        CSF_AT(uint8_t, self, 0x187) = 0;
    } else if (CSF_AT(uint8_t, self, 0x142)) {
        Show(CSF_AT(void*, self, 0x18), SpecialPage());
    } else if (CSF_AT(uint8_t, self, 0x271)) {
        CSF_AT(uint8_t, self, 0x270) = 1;
        CSF_AT(void*, Find(CSF_AT(void*, self, 0x18), ConfirmPage()), 0x20) = self + 0x270;
        Show(CSF_AT(void*, self, 0x18), ConfirmPage());
    } else {
        Show(CSF_AT(void*, self, 0x18), DefaultPage());
    }
}

static void __fastcall LeaveLevel(uint8_t* self, void*) {
    CSF_AT(uint8_t, self, 0x17C) = 1;
    void* app = App_Get();
    Slot<void(__fastcall*)(void*, void*, int)>(app, 0x5C)(app, nullptr, 1);
    ResetApp(App_Get(), nullptr);
    enter_level::ResetThirdManager(enter_level::GetGlobal_521BB0(), nullptr);
    UnregisterSound(enter_level::GetGlobal_521BB0());
    void* loader = enter_level::GetGlobal_8B0568();
    if (CSF_AT(uint8_t, loader, 0xB5)) {
        loader = enter_level::GetGlobal_8B0568();
        Slot<void(__fastcall*)(void*, void*)>(loader, 0x2C)(loader, nullptr);
    }
    PrepareSceneState(GetSceneState(), nullptr);
    ResetScene(enter_level::GetGlobal_8B0568(), nullptr);
    reinterpret_cast<Getter>(CSF_ADDR_FN(0x004506B0))();
    reinterpret_cast<Getter>(CSF_ADDR_FN(0x00519400))();
    uint8_t* page = self - 4;
    ClearControls(page, nullptr);
    Slot<void(__fastcall*)(void*, void*)>(page, 0x78)(page, nullptr);
    // 542720 is cdecl/ret, despite the unused ECX load from self+148.
    ExitCharacterSelect();
    const uintptr_t releases[] = {CSF_ADDR_FN(0x00525140), CSF_ADDR_FN(0x004F4930),
        CSF_ADDR_FN(0x0044B8A0), CSF_ADDR_FN(0x004FBDF0), CSF_ADDR_FN(0x00436070),
        CSF_ADDR_FN(0x00442790), CSF_ADDR_FN(0x0051F750), CSF_ADDR_FN(0x004010A0),
        CSF_ADDR_FN(0x005189E0), CSF_ADDR_FN(0x00519200), CSF_ADDR_FN(0x00519280),
        CSF_ADDR_FN(0x004E8C60), CSF_ADDR_FN(0x00506AC0), CSF_ADDR_FN(0x00457A90),
        CSF_ADDR_FN(0x006C8C10)};
    for (uintptr_t fn : releases) reinterpret_cast<Getter>(fn)();
    ResetPortraits(enter_level::Scheduler_BoundContexts(enter_level::GetGlobal_8B0568(), nullptr), nullptr);
    ReleaseRegistration(enter_level::DefaultDestination(enter_level::GetGlobal_8B0568(), nullptr), nullptr);
    reinterpret_cast<Getter>(CSF_ADDR_FN(0x004755F0))();
    void* controller = GetSceneController(Render_GetSystem(), nullptr);
    Slot<void(__fastcall*)(void*, void*)>(controller, 0x28)(controller, nullptr);
    char* scenario = CSF_AT(uint32_t, self, 0x174) < 16
        ? reinterpret_cast<char*>(self + 0x160) : CSF_AT(char*, self, 0x160);
    SelectScenario(enter_level::GetGlobal_8B0568(), nullptr, scenario, 1);
    // Both words survive the argumentless database getter (SetTextPath ret 8).
    SetTextPath(GetTextDatabase(), nullptr, 4, CSF_STR(0x0082E65C, ""));
    if (CSF_AT(uint8_t, App_Get(), 0xD0))
        CSF_AT(uint8_t, enter_level::GetGlobal_75FDF0(), 0xCC) = CSF_AT(uint8_t, self, 0x270);
    OpenNextPage(page, nullptr);
}
} // namespace leave_level
HOOK(0x00536F20, leave_level::OpenNextPage, "auto");
HOOK(0x00537490, leave_level::LeaveLevel, "auto");

// Standalone-only pre-SEH diagnostics; these do not substitute a value or
// suppress the original parser exception. No static state/initialization.
void csf_script_loaded(const char* path, unsigned size, void* root) {
    std::fprintf(stderr, "csf: SCRIPT loaded path=%s size=%u root=%p\n", path, size, root);
    std::fflush(stderr);
}
void csf_script_payload_failed(void* value, int type) {
    std::fprintf(stderr, "csf: SCRIPT payload allocation failed value=%p type=%d\n", value, type);
    std::fflush(stderr);
}
void csf_script_invalid_leaf(const char* parent, unsigned index, unsigned count,
                             void* node, int nodeType, void* entry, unsigned opcode,
                             void* value, const uint32_t* definition) {
    std::fprintf(stderr,
        "csf: SCRIPT invalid leaf parent=%s child=%u/%u node=%p nodeType=%d "
        "entry=%p opcode=%u value=%p payload=null def=(%u,%u,%u)\n",
        parent, index, count, node, nodeType, entry, opcode, value,
        definition[0], definition[1], definition[2]);
    std::fflush(stderr);
}
