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

// __cdecl, no arguments; returns the global scheduler state object in eax.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// __thiscall on the scheduler base (object in ecx); returns the scheduler proxy kind in eax.
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));

// If the scheduler proxy is one of the special kinds (0x10, 0x800, 0x1000), marks
// this+0x38 as -1 and invokes the virtual slot at +0xA8 on the object stored at
// this+0x24 with argument 0. Returns the virtual call's eax result, otherwise the
// proxy kind unchanged.
static int __fastcall CMenuProfiles_CheckSchedulerProxyAndDisable(void* self, void* /*edx*/)
{
    void* schedulerBase = reinterpret_cast<void*>(GetGlobalState_50D190());
    int proxyKind = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);

    if (proxyKind == 0x10 || proxyKind == 0x800 || proxyKind == 0x1000)
    {
        void* targetObject = CSF_AT(void*, self, 0x24);
        CSF_AT(int, self, 0x38) = -1;
        void** vtable = CSF_AT(void**, targetObject, 0);
        return ((int(__fastcall*)(void*, void*, int))vtable[0xA8 / 4])(targetObject, nullptr, 0);
    }
    return proxyKind;
}

HOOK(0x0053BAA0, CMenuProfiles_CheckSchedulerProxyAndDisable, "auto");
