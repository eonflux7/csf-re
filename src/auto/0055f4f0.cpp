#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Scheduler/global-state helpers referenced by this dispatcher.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

// Resolves the scheduler proxy for the current global state. Returns the
// proxy-state code (0x1000 / 0x800 paths dispatch through vtables; otherwise
// the raw proxy value is returned). Callee pops 4 bytes (one stack arg a2).
static int __fastcall MenuMpOnline_ResolveSchedulerProxy(void* self, void* /*edx*/, int a2)
{
    (void)a2;

    int schedulerBase = GetGlobalState_50D190();
    int proxy = SchedulerBase_GetSchedulerProxy(reinterpret_cast<void*>(schedulerBase), nullptr);
    if (proxy == 0x1000)
    {
        int host = GetGlobalState_6A6D70();
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, host, 0)[140 / 4])(
            reinterpret_cast<void*>(host), nullptr);
    }

    schedulerBase = GetGlobalState_50D190();
    proxy = SchedulerBase_GetSchedulerProxy(reinterpret_cast<void*>(schedulerBase), nullptr);
    if (proxy == 0x800)
    {
        int host = GetGlobalState_6A6D70();
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, host, 0)[172 / 4])(
            reinterpret_cast<void*>(host), nullptr);
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[76 / 4])(self, nullptr);
    }

    return proxy;
}

HOOK(0x0055F4F0, MenuMpOnline_ResolveSchedulerProxy, "auto");
