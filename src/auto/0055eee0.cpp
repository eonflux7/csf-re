#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, GetSchedulerProxyKind, 0x00734A10, (void* schedulerBase, void* /*edx*/));

// Set by the dispatch helper when the 0x1000 path requires the tail jump.
static int g_tailJumpPending = 0;
// Global state object whose vtable slot +0x8c receives the tail jump.
static void* g_tailJumpObject = nullptr;

// Non-naked body of the original function (everything except the tail jump).
// Receives ecx = this (fastcall convention, edx unused), returns the eax value the
// original would produce on its non-tail paths.
static int __fastcall MenuMpOnline_DispatchSchedulerProxy(void* self, void* /*edx*/)
{
    g_tailJumpPending = 0;
    g_tailJumpObject = nullptr;

    int schedulerBase = GetGlobalState_50D190();
    int proxyKind = GetSchedulerProxyKind(reinterpret_cast<void*>(schedulerBase), nullptr);

    if (proxyKind == 0x800) {
        int globalState = GetGlobalState_6A6D70();
        // vtbl+0x90 is slot 36.
        int callResult = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, globalState, 0)[36]))(
            reinterpret_cast<void*>(globalState), nullptr);
        CSF_AT(uint32_t, self, 0x18) = 4;
        return callResult;
    }
    if (proxyKind == 0x1000) {
        int globalState = GetGlobalState_6A6D70();
        g_tailJumpObject = reinterpret_cast<void*>(globalState);
        g_tailJumpPending = 1;
        return proxyKind;
    }
    return proxyKind;
}

// Pointer to the helper, so the naked entry point can call it through a register.
static int(__fastcall* g_dispatchHelper)(void*, void*) = &MenuMpOnline_DispatchSchedulerProxy;

// Naked entry point. Calls the helper with the stack untouched, then either returns
// its eax or performs the indirect tail jump through vtbl+0x8c with the caller's
// stack arguments and registers forwarded unchanged.
static __declspec(naked) void MenuMpOnline_DispatchSchedulerProxyThunk()
{
    __asm {
        mov eax, dword ptr [g_dispatchHelper]
        call eax
        cmp dword ptr [g_tailJumpPending], 0
        je ThunkReturn
        mov ecx, dword ptr [g_tailJumpObject]
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x8c]
    ThunkReturn:
        ret
    }
}

HOOK(0x0055EEE0, MenuMpOnline_DispatchSchedulerProxyThunk, "auto");
