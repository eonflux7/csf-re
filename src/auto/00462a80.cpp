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

// Returns the index of the first 44-byte entry in the table whose leading
// dword equals `value`, or -1 if no entry matches. The table is delimited by
// this->begin (+0x38) and this->end (+0x3C).
static int __fastcall S_00692b80_FindIndexOfValue(S_00692b80* self, void* /*edx*/, int value)
{
    // The null check uses the begin pointer captured on entry, as the original does.
    const uint32_t tableBegin = CSF_AT(uint32_t, self, 0x38);
    uint32_t index = 0;
    uint32_t entryOffset = 0;

    while (tableBegin != 0)
    {
        // Entry count is recomputed on every iteration from the current end pointer.
        const int32_t entryCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x3C) - tableBegin) / 44;

        // Unsigned comparison, matching the original jae.
        if (!(index < static_cast<uint32_t>(entryCount)))
            break;

        // The element base is reloaded from this->begin on every iteration.
        const uint32_t currentBegin = CSF_AT(uint32_t, self, 0x38);
        const int entryKey = *reinterpret_cast<int*>(
            static_cast<uintptr_t>(currentBegin + entryOffset));

        if (value == entryKey)
            return static_cast<int>(index);

        ++index;
        entryOffset += 44;
    }

    return -1;
}

HOOK(0x462A80, S_00692b80_FindIndexOfValue, "auto");
