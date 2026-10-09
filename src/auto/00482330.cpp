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

// Callee: C_008319c0::GetSubObjectField74 (__fastcall, one arg in ecx, ret 0)
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

// Callee: CCharWeaponFsm::IsField000cNull (tail-jumped to by the original, ret 0)
CSF_FUNC(int, __fastcall, CCharWeaponFsm_IsField000cNull, 0x0047F9B0, (CCharWeaponFsm* self, void*));

// Returns true when the action FSM has an active sub-object whose state is ready
// for action: either the sub-object's field 74 is 10 while field_000c is 7, or
// the generic weapon-FSM check passes. Returns false when there is no sub-object.
static bool __fastcall CCharActionFsm_IsActionStateReady(CCharActionFsm* self, void* /*edx*/)
{
    bool hasSubObject = self->vGetIndexedEntryOrZero_37() != 0;
    if (!hasSubObject)
        return false;

    if (self->m_stateCode == 7)
    {
        C_008319c0* subObject = reinterpret_cast<C_008319c0*>(self->vGetIndexedEntryOrZero_37());
        if (C_008319c0_GetSubObjectField74(subObject, nullptr) == 10)
            return true;
    }

    // Original tail-jumps here: forwards `this` in ecx, result is the callee's.
    return CCharWeaponFsm_IsField000cNull(self, nullptr) != 0;
}

HOOK(0x482330, CCharActionFsm_IsActionStateReady, "auto");
