#include <cstdint>
#include "globals.h"
#include "hook.h"

// Concurrency runtime helpers bound by address.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

// Original: __thiscall (this in ecx, one stack arg), ret 4.
// Checks the scheduler proxy value: 0x1000 sets a flag byte at +0x48 and dword at +0x18 = 2;
// 0x800 dispatches through the object's vtable slot at +0x4C.
static int __fastcall MenuMpOnline_CheckSchedulerProxyAndDispatch(uint8_t* self, void* /*edx*/, int /*unusedArg*/)
{
    int result = SchedulerBase_GetSchedulerProxy(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    if (result == 0x1000)
    {
        CSF_AT(uint8_t, self, 0x48) = 1;
        CSF_AT(uint32_t, self, 0x18) = 2;
        return result;
    }

    result = SchedulerBase_GetSchedulerProxy(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    if (result == 0x800)
    {
        void** vtbl = CSF_AT(void**, self, 0);
        return ((int(__fastcall*)(void*, void*))vtbl[0x4C / 4])(self, nullptr);
    }

    return result;
}

HOOK(0x0055F470, MenuMpOnline_CheckSchedulerProxyAndDispatch, "auto");
