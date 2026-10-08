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

// Returns the scheduler base singleton (cdecl, no args). Original: sub_50D190.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Returns the global game state object (cdecl, no args). Original: sub_6A6D70.
CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());

// SchedulerBase::GetSchedulerProxy. Original: sub_734A10 (this in ecx, no stack args).
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

// Dispatches on the scheduler proxy state. Returns 0x1000 -> virtual slot 59 on the
// global state; 0x800 -> virtual slot 17 on the global state with arg 1, then
// virtual slot 19 on this; otherwise returns the proxy value as-is.
static int __fastcall CMenuMpOnline_DispatchOnSchedulerProxy(void* self, void* /*edx*/, int a2)
{
    (void)a2;

    void* schedulerBase = GetGlobalState_50D190();
    int proxyState = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);
    if (proxyState == 0x1000)
    {
        void* globalState = GetGlobalState_6A6D70();
        return ((int(__fastcall*)(void*, void*))(CSF_AT(void**, globalState, 0)[59]))(globalState, nullptr);
    }

    schedulerBase = GetGlobalState_50D190();
    proxyState = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);
    if (proxyState == 0x800)
    {
        void* globalState = GetGlobalState_6A6D70();
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, globalState, 0)[17]))(globalState, nullptr, 1);
        return ((int(__fastcall*)(void*, void*))(CSF_AT(void**, self, 0)[19]))(self, nullptr);
    }

    return proxyState;
}

HOOK(0x0055F3B0, CMenuMpOnline_DispatchOnSchedulerProxy, "auto");
