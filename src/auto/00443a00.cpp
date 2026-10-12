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

// Scans the 40-byte entry table at this->field_000c (count this->field_0014).
// An entry matches when its active byte (+0) is nonzero and its dword at +8
// equals the dword at +4 of the key object a2. Returns the matching index, or -1.
static int __fastcall S_00443a00_FindEntryIndex(S_00443a00* self, void* /*edx*/, int a2)
{
    const uint32_t entryCount = self->field_0014;
    int result = -1;

    if (entryCount == 0)
        return result;

    uint32_t index = 0;
    uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_000c);

    do {
        if (CSF_AT(uint8_t, entry, 0) != 0 &&
            CSF_AT(uint32_t, entry, 8) == CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a2), 4))
        {
            return static_cast<int>(index);
        }
        ++index;
        entry += 40;
    } while (index < entryCount);

    return result;
}

HOOK(0x443A00, S_00443a00_FindEntryIndex, "auto");
