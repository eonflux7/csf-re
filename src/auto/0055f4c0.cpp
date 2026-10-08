#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0050D190 - returns the global concurrency state object.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// 0x00734A10 - __thiscall, this in ecx, no stack arguments (ret 0).
CSF_FUNC(void*, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));

// 0x0055F4C0
// Resolves the current scheduler proxy. If the proxy is the 0x1000 sentinel,
// it falls back to the virtual method at vtable slot 19 (offset 0x4c) on `this`.
static void* __fastcall CMenuMpOnline_ResolveSchedulerProxy(void* self, void* /*edx*/, int /*a2*/)
{
    void* globalState = GetGlobalState_50D190();
    void* proxy = SchedulerBase_GetSchedulerProxy(globalState, nullptr);

    if (proxy == reinterpret_cast<void*>(0x1000))
    {
        void** vtbl = *reinterpret_cast<void***>(self);
        return reinterpret_cast<void* (__fastcall*)(void*, void*)>(vtbl[0x4c / 4])(self, nullptr);
    }

    return proxy;
}

HOOK(0x0055F4C0, CMenuMpOnline_ResolveSchedulerProxy, "auto");
