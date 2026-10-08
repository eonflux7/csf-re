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

// Callee: __cdecl, no arguments, returns the global scheduler state object.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Callee: __thiscall (ecx = scheduler base), no stack arguments, returns the proxy kind.
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

// Dispatches on the scheduler proxy kind. Kind 4 calls vtable slot 0x58 on the owner;
// kind 8 sets the owner's flag at +0x28 to 1 and calls vtable slot 0x60.
// Any other kind is returned unchanged.
static int __fastcall MenuAccounts_DispatchByProxyKind(void* self, void* /*edx*/, int /*unusedStackArg*/)
{
    void* schedulerBase = GetGlobalState_50D190();
    int proxyKind = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);

    if (proxyKind == 4)
    {
        // vtable slot 0x58 / 4 = 22, no arguments besides this
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x58 / 4])(self, nullptr);
    }
    if (proxyKind == 8)
    {
        CSF_AT(int32_t, self, 0x28) = 1;
        // vtable slot 0x60 / 4 = 24, no arguments besides this
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x60 / 4])(self, nullptr);
    }
    return proxyKind;
}

HOOK(0x00564D50, MenuAccounts_DispatchByProxyKind, "auto");
