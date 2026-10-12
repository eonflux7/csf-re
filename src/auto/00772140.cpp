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

// Searches the animation entry table (reached through the pointer at field_0004) for the
// entry whose leading ID dword equals animId.
// Entries are 96 (0x60) bytes each, stored in the range [begin, end) at table+4 / table+8.
// Returns the entry index, or -1 if the table is empty or the ID is not present.
static int __fastcall C_00871a18_FindAnimIndexById(C_00871a18* self, void* /*edx*/, int animId)
{
    uint8_t* tableHeader = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
    uint32_t firstEntryAddr = CSF_AT(uint32_t, tableHeader, 0x4);

    int32_t entryCountSigned = 0;
    if (firstEntryAddr != 0)
    {
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, tableHeader, 0x8) - firstEntryAddr);
        entryCountSigned = byteSpan / 96;
    }
    uint32_t entryCount = static_cast<uint32_t>(entryCountSigned);

    if (entryCount == 0)
        return -1;

    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstEntryAddr));
    for (uint32_t index = 0; index < entryCount; ++index)
    {
        if (CSF_AT(int, entry, 0) == animId)
            return static_cast<int>(index);
        entry += 0x60;
    }
    return -1;
}

HOOK(0x772140, C_00871a18_FindAnimIndexById, "auto");
