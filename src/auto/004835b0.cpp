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

CSF_FUNC(int, __fastcall, sub_480860, 0x00480860, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_494E90, 0x00494E90, (void* self, void* edx, int a4));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));

static void __fastcall CCharActionFsm_UpdateWhileTargetValid(CCharActionFsm* self, void* /*edx*/, int a2)
{
    if (self->field_0024 == 0xFFFFFFFFu)
        return;

    if (self->m_stateCode == 4)
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4) &= ~0x10u;

    if (C_004950e0_GetField398(reinterpret_cast<void*>(self->field_0008), nullptr))
    {
        if (C_004950e0_GetField398(reinterpret_cast<void*>(self->field_0008), nullptr))
        {
            sub_494E90(reinterpret_cast<void*>(self->field_0008), nullptr, 0);
            self->vBeginWeaponStateChange(2, 0);
        }
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4) &= ~0x10u;
    }
    else
    {
        sub_480860(self, nullptr, a2);
    }
}

HOOK(0x004835B0, CCharActionFsm_UpdateWhileTargetValid, "auto");
