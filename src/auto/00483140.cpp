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

CSF_FUNC(int, __fastcall, GetMemberValueD4, 0x00476090, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static char __fastcall CCharActionFsm_UpdateAimState(CCharActionFsm* self, void* /*edx*/)
{
    int actorHandle = self->vGetIndexedEntryOrZero();
    if (GetMemberValueD4(reinterpret_cast<void*>(static_cast<uintptr_t>(actorHandle)), nullptr) == 1)
    {
        if (Hud_IsSuspended(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr))
        {
            int globalObject = GetGlobalValue_8BC1E0();
            self->field_00ac = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(globalObject)), 0x958);
        }
        else
        {
            self->field_00ac = 0x3F000000u;
        }
        return static_cast<char>(self->vBeginWeaponStateChange(16, 15));
    }

    char result = static_cast<char>(self->m_pendingActionFlag);
    if (result)
    {
        return static_cast<char>(self->vBeginWeaponStateChange(5, 0));
    }
    return result;
}

HOOK(0x00483140, CCharActionFsm_UpdateAimState, "auto");
