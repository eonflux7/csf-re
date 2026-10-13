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

// Returns the entry at `index` in the RtFS manager's entry table, or 0 when
// `index` is not below the entry count. Each entry is 72 bytes.
// Manager layout (no declared class): +0x04 entry count, +0x58 entry table base.
static int __cdecl RtFS_GetEntryAtIndex(int managerPtr, unsigned int index)
{
    void* manager = reinterpret_cast<void*>(managerPtr);
    const uint32_t entryCount = CSF_AT(uint32_t, manager, 0x04);
    if (index >= entryCount)
        return 0;

    const uint32_t entryTableBase = CSF_AT(uint32_t, manager, 0x58);
    return static_cast<int>(entryTableBase + 72u * index);
}

HOOK(0x0069FFE0, RtFS_GetEntryAtIndex, "auto");
