#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Scheduler helpers reached by address.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

// Menu-side handler: queries the scheduler proxy state; on proxy value 4 it dispatches
// through the object's vtable slot 0x54, on value 8 it raises the flag at +0x280.
static int __fastcall MenuAccounts_ProcessSchedulerProxyState(void* self, void* /*edx*/, int /*unused*/)
{
    void* schedulerBase = GetGlobalState_50D190();
    int result = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);

    if (result == 4) {
        using VirtualFn = int(__fastcall*)(void*, void*);
        VirtualFn fn = reinterpret_cast<VirtualFn>(CSF_AT(void**, self, 0)[0x54 / 4]);
        return fn(self, nullptr);
    }
    if (result == 8) {
        CSF_AT(int, self, 0x280) = 1;
    }
    return result;
}

HOOK(0x0055C320, MenuAccounts_ProcessSchedulerProxyState, "auto");
