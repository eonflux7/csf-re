#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __cdecl, NullSub_506AC0, 0x506AC0, ());
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x50D190, ());
CSF_FUNC(void*, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuAccounts_HandleStateOne, 0x5650F0, (CMenuAccounts* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CMenuAccounts_DispatchByProxyKind, 0x564D50, (CMenuAccounts* self, void* edx, int a2));

static int __fastcall CMenuAccounts_ProcessSchedulerProxyState(CMenuAccounts* self, void* /*edx*/, int a2)
{
    NullSub_506AC0();

    const uint32_t state = self->field_0028;
    int result = static_cast<int>(state);

    switch (state) {
    case 0u:
        self->field_0028 = 1;
        break;

    case 1u:
        result = CMenuAccounts_HandleStateOne(self, nullptr, a2);
        break;

    case 2u:
        result = CMenuAccounts_DispatchByProxyKind(self, nullptr, a2);
        break;

    case 3u:
    case 4u: {
        void* globalState = GetGlobalState();
        result = static_cast<int>(reinterpret_cast<uintptr_t>(
            SchedulerBase_GetSchedulerProxy(globalState, nullptr)));
        if (result != 0) {
            result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[24])(self, nullptr);
            self->field_0028 = 1;
        }
        break;
    }

    default:
        break;
    }

    return result;
}

HOOK(0x005653F0, CMenuAccounts_ProcessSchedulerProxyState, "auto");
