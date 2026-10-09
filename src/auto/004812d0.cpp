#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

// Scans the sub-object pointer array [field_0018, field_001c) and returns the index of the
// first element whose GetSubObjectField74 value equals targetValue, or -1 when none matches.
static int __fastcall CCharWeaponFsm_FindSubObjectIndex(CCharWeaponFsm* self, void* /*edx*/, int targetValue)
{
    for (uint32_t i = 0; ; ++i)
    {
        // The original reloads the array start on every iteration.
        uint32_t arrayBegin = self->field_0018;
        if (arrayBegin == 0)
            break;

        int byteSpan = static_cast<int>(self->field_001c - arrayBegin);
        uint32_t elementCount = static_cast<uint32_t>(byteSpan >> 2);
        if (i >= elementCount)
            break;

        C_008319c0* element = reinterpret_cast<C_008319c0**>(arrayBegin)[i];
        if (C_008319c0_GetSubObjectField74(element, nullptr) == targetValue)
            return static_cast<int>(i);
    }
    return -1;
}

HOOK(0x004812D0, CCharWeaponFsm_FindSubObjectIndex, "auto");
