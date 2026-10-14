// Original game behavior for slots whose DLL hooks add developer/test options.
// Only csf_standalone_startup compiles this file; the DLL uses core/devcmd.cpp.
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cstring>
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
    return real ? real(inst, ver, riid, out, outer) : E_FAIL;
}

HOOK(0x4FB8F0, DevCommandLine_Set, "auto");
HOOK(0x508F50, Window_FitClientArea, "auto");
HOOK(0x5364D0, GameScreen_HasBriefing, "auto");
HOOK(0x7DB650, Import_DirectInput8Create, "boundary-import");
