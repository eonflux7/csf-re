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

// Looks up the entry index for a2 in this object's table; returns -1 if absent.
CSF_FUNC(int, __fastcall, C_00863044_FindEntryIndex, 0x006FE4F0, (void* self, void* edx, int a2));

// Returns the 32-bit value stored at offset +8 of the entry (12-byte records)
// matched by a2, or 0 when no entry matches.
static int __fastcall C_00863044_GetEntryValueByKey(C_00863044* self, void* /*edx*/, int a2)
{
    int entryIndex = C_00863044_FindEntryIndex(self, nullptr, a2);
    if (entryIndex == -1)
        return 0;

    uint8_t* entryTable = reinterpret_cast<uint8_t*>(self->field_0024);
    return CSF_AT(int, entryTable, 12 * entryIndex + 8);
}

HOOK(0x006FE8C0, C_00863044_GetEntryValueByKey, "auto");
