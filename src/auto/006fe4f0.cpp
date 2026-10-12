#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scans the 12-byte record table at this->field_0024 (count this->field_002c)
// and returns the index of the first record whose leading dword equals key.
// Returns -1 when no record matches.
static int __fastcall C_00863044_FindFirstIndexByKey(C_00863044* self, void* /*edx*/, int key)
{
    const uint32_t recordCount = self->field_002c;
    int resultIndex = -1;

    for (uint32_t index = 0; index < recordCount; ++index)
    {
        const uint8_t* recordBase =
            reinterpret_cast<const uint8_t*>(self->field_0024) + static_cast<size_t>(index) * 12;
        if (*reinterpret_cast<const int*>(recordBase) == key)
        {
            resultIndex = static_cast<int>(index);
            break;
        }
    }

    return resultIndex;
}

HOOK(0x006FE4F0, C_00863044_FindFirstIndexByKey, "auto");
