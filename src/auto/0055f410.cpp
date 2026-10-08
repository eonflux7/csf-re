#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10,
         (void* self, void* edx));

static int __fastcall CMenuMpOnline_DispatchOnlineState(
    void* self, void* /*edx*/, int /*unused*/)
{
    void* schedulerBase = GetGlobalState_50D190();
    int state = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);

    if (state == 0x1000)
    {
        void* globalState = GetGlobalState_6A6D70();
        return ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, globalState, 0)[34])(globalState, nullptr);
    }

    schedulerBase = GetGlobalState_50D190();
    state = SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr);

    if (state == 0x800)
    {
        void* globalState = GetGlobalState_50D190();
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, globalState, 0)[26])(globalState, nullptr, 1);

        return ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[19])(self, nullptr);
    }

    return state;
}

HOOK(0x0055F410, CMenuMpOnline_DispatchOnlineState, "auto");
