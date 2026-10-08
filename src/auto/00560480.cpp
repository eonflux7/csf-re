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

// Original functions, bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x50D190, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));

// Original globals.
CSF_GLOBAL(const char*, g_pszMenu_P_LbCMch, 0x899B50);
CSF_GLOBAL(uint32_t, dword_89A5F8, 0x89A5F8);

// Hand-off slot between the helper and the naked thunk. Holds the menu object
// when the original would perform its indirect tail jump, nullptr otherwise.
static void* g_pendingTailTarget = nullptr;

// Body of the original, minus the tail jump. Returns the value the original
// would leave in eax on its normal return path. Sets g_pendingTailTarget when
// the original would tail-jump through [this->vtbl + 0x4c].
static int __fastcall CMenuMpSessions_ResolveDispatch(CMenuMpSessions* self, void* /*edx*/)
{
    void* globalState = GetGlobalState_50D190();
    int schedulerProxyCode = SchedulerBase_GetSchedulerProxy(globalState, nullptr);

    int stepMinusOne = schedulerProxyCode - 1;
    if (stepMinusOne == 0)
    {
        // Init path: notify the child at +0x44, then store this->+0x18 globally.
        g_pendingTailTarget = nullptr;

        void* menuChild = reinterpret_cast<void*>(self->field_0044);
        void** childVtbl = *reinterpret_cast<void***>(menuChild);
        ((void(__fastcall*)(void*, void*, void*, int))childVtbl[0x60 / 4])(
            menuChild, nullptr, self, 0);

        menuChild = reinterpret_cast<void*>(self->field_0044);
        const char* menuLabel = g_pszMenu_P_LbCMch;
        childVtbl = *reinterpret_cast<void***>(menuChild);
        ((void(__fastcall*)(void*, void*, const char*))childVtbl[0x58 / 4])(
            menuChild, nullptr, menuLabel);

        uint32_t savedValue = self->field_0018;
        dword_89A5F8 = savedValue;
        return static_cast<int>(savedValue);
    }

    int result = stepMinusOne - 1;
    if (result == 0)
    {
        // Tail-jump case: the thunk performs jmp [eax+0x4c] on this object.
        g_pendingTailTarget = self;
        return result;
    }

    g_pendingTailTarget = nullptr;
    return result;
}

// Helper pointer, reached from the thunk through a register call.
static int(__fastcall* g_resolveDispatch)(CMenuMpSessions*, void*) = &CMenuMpSessions_ResolveDispatch;

// Entry point. Calls the helper through a register with ecx=this, then either
// tail-jumps through the object's vtable slot 0x4c (forwarding the caller's
// stack untouched) or returns with the helper's result in eax.
static __declspec(naked) void CMenuMpSessions_DispatchThunk()
{
    __asm {
        mov eax, dword ptr [g_resolveDispatch]
        call eax
        mov ecx, dword ptr [g_pendingTailTarget]
        test ecx, ecx
        je L_ReturnNormally
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x4c]
    L_ReturnNormally:
        ret
    }
}

HOOK(0x560480, CMenuMpSessions_DispatchThunk, "auto");
