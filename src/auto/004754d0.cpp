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

// Scene-entry table: pointer to the uint32_t item array and its live item count.
CSF_GLOBAL(uint32_t*, g_SceneEntryItems, 0x00899744);
CSF_GLOBAL(uint32_t, g_SceneEntryCount, 0x0089974C);

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the first entry equal to `objectHandle` from the scene-entry table, shifting the tail down.
// Returns the index where the entry was found (or the count if not found), or the memcpy result when shifting.
static unsigned int HudUnregisterObject(int objectHandle)
{
    unsigned int result = 0;
    uint32_t count = g_SceneEntryCount;
    if (count == 0)
        return result;

    uint32_t* entries = g_SceneEntryItems;
    while (static_cast<int>(entries[result]) != objectHandle)
    {
        if (++result >= count)
            return result;
    }

    uint32_t newCount = count - 1;
    g_SceneEntryCount = newCount;
    if (result != newCount)
    {
        std::size_t tailBytes = static_cast<std::size_t>(newCount - result) * 4;
        return static_cast<unsigned int>(reinterpret_cast<uintptr_t>(
            _memcpy(&entries[result], &entries[result + 1], tailBytes)));
    }
    return result;
}

HOOK(0x4754D0, HudUnregisterObject, "auto");
