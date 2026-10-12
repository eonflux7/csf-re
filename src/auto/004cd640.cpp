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

// Callee bound by address (__fastcall, ret 4: self, edx, value).
CSF_FUNC(int, __fastcall, WeaponFirearms_SetCurrentAndPreviousValue, 0x004762E0, (void* self, void* , int value));

// Searches the sweep owner's controller for a target and, if one is found,
// hands it to the controller and switches the weapon state. The object's
// state field is then moved to 3. States 2, 3 and 4 are skipped entirely.
static int __fastcall C_0083ab58_ProcessSearchSweepTarget(C_0083ab58* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0040);

    if (self->field_0040 != 3 && self->field_0040 != 4 && self->field_0040 != 2)
    {
        uint8_t* sweepOwner = reinterpret_cast<uint8_t*>(self->field_0014);
        void* controller = reinterpret_cast<void*>(CSF_AT(uint32_t, sweepOwner, 0x44));

        // slot 252 / 4 = 63: query for the next target; -1 means none.
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[63])(controller, nullptr);

        if (result != -1)
        {
            // slot 200 / 4 = 50: hand the target to the controller.
            result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, controller, 0)[50])(controller, nullptr, result);
            if (result)
            {
                result = WeaponFirearms_SetCurrentAndPreviousValue(reinterpret_cast<void*>(result), nullptr, 2);
            }
            self->field_0040 = 3;
        }
    }

    return result;
}

HOOK(0x004CD640, C_0083ab58_ProcessSearchSweepTarget, "auto");
