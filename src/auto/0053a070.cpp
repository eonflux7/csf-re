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

// Original game functions bound by address.
CSF_FUNC(int, __cdecl, CMenuPause_GetGlobalValue, 0x521BB0, ());
CSF_FUNC(void, __fastcall, CMenuPause_ApplyGlobalValue, 0x5231D0, (int self, void* edx, int a2));
CSF_FUNC(void, __cdecl, CMenuPause_NullSub, 0x506AC0, ());
CSF_FUNC(int, __fastcall, CMenuPause_RunStateZeroAction, 0x539EF0, (int* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuPause_RunStateOneAction, 0x539FB0, (int* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuPause_RunStateThreeAction, 0x539E50, (int* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, CMenuPause_RunStateFourAction, 0x539EA0, (int* self, void* edx, int a3));

// Dispatches the pending pause-menu action stored in field_0014.
// a2 is stored as raw bits into field_003c and forwarded to the action handlers.
static int __fastcall CMenuPause_DispatchPendingAction(CMenuPause* self, void* /*edx*/, int a2)
{
    self->field_003c = static_cast<uint32_t>(a2);

    int globalValue = CMenuPause_GetGlobalValue();
    CMenuPause_ApplyGlobalValue(globalValue, nullptr, a2);
    CMenuPause_NullSub();

    uint32_t state = self->field_0014;
    switch (state)
    {
    case 0u:
        return CMenuPause_RunStateZeroAction(reinterpret_cast<int*>(self), nullptr);
    case 1u:
        return CMenuPause_RunStateOneAction(reinterpret_cast<int*>(self), nullptr);
    case 2u:
    {
        uint32_t count = self->field_0040 + 1u;
        self->field_0040 = count;
        if (count >= 3u)
        {
            void** vtbl = CSF_AT(void**, self, 0);
            self->field_0014 = 0xFFFFFFFFu;
            auto virtualFn = reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[21]);
            return virtualFn(self, nullptr);
        }
        return static_cast<int>(count);
    }
    case 3u:
        return CMenuPause_RunStateThreeAction(reinterpret_cast<int*>(self), nullptr, a2);
    case 4u:
        return CMenuPause_RunStateFourAction(reinterpret_cast<int*>(self), nullptr, a2);
    default:
        return static_cast<int>(state);
    }
}

HOOK(0x53A070, CMenuPause_DispatchPendingAction, "auto");
