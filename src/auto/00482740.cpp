#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: sub_498F50, __fastcall (ecx = self, edx unused, two stack args), ret 8.
CSF_FUNC(int, __fastcall, WeaponFsm_SubmitStateCode, 0x00498F50, (void* self, void* edx, int a2, char a3));

// Original 0x482740: __thiscall with one stack argument (ret 4).
// Submits state code 0x4000 first. Unless the code is 7, it clears bit 0x10 in the
// owner's flags at +0x2E4 and submits state code 16. Returns the result of the last submit.
static int __fastcall CCharActionFsm_HandleStateCode(CCharActionFsm* self, void* /*edx*/, int stateCode)
{
    // ecx for the first submit is the owner object stored at this->field_0008.
    int result = WeaponFsm_SubmitStateCode(reinterpret_cast<void*>(self->field_0008), nullptr, 0x4000, 0);

    if (stateCode != 7)
    {
        void* owner = reinterpret_cast<void*>(self->field_0008);
        CSF_AT(uint32_t, owner, 0x2E4) &= ~0x10u;

        result = WeaponFsm_SubmitStateCode(reinterpret_cast<void*>(self->field_0008), nullptr, 16, 0);
    }

    return result;
}

HOOK(0x482740, CCharActionFsm_HandleStateCode, "auto");
