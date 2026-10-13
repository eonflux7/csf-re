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

// Array growth helper (S_0041e5f0 container). Same bind as its other callers.
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Globals of the scene-entries array container (g_SceneEntries).
CSF_GLOBAL(uint32_t, g_SceneEntriesData, 0x00899744);       // field_0014: element storage pointer
CSF_GLOBAL(uint32_t, g_SceneEntriesCapacity, 0x00899748);   // field_0018: capacity
CSF_GLOBAL(uint32_t, g_SceneEntriesCount, 0x0089974C);      // field_001c: element count
CSF_GLOBAL(uint32_t, g_SceneEntriesGrowStep, 0x00899750);   // field_0020: growth amount

// Vtable referenced by the container's grow call site.
static void* const g_SceneEntriesVtbl = CSF_VTBL(0x0041E5F0);

// Returns the index of a1 when already registered; otherwise appends it and
// returns the address of the new slot. Mirrors the original's return values.
static uint32_t MpSpawnPoints_RegisterObject(int a1)
{
    uint32_t count = g_SceneEntriesCount;
    uint32_t entriesAddr = g_SceneEntriesData;
    uint32_t index = 0;

    if (count != 0)
    {
        do
        {
            if (*(reinterpret_cast<int*>(static_cast<uintptr_t>(entriesAddr)) + index) == a1)
                return index;
            ++index;
        } while (index < count);
    }

    // Not found: grow if full, then append.
    if (count >= g_SceneEntriesCapacity)
    {
        auto* self = reinterpret_cast<S_0041e5f0*>(CSF_ADDR_DATA(0x00899744));
        S_0041e5f0_GrowArrayCapacity(self, nullptr, static_cast<int>(g_SceneEntriesGrowStep));
        count = g_SceneEntriesCount;
        entriesAddr = g_SceneEntriesData;
    }

    uint32_t slotAddr = entriesAddr + count * 4;
    g_SceneEntriesCount = count + 1;
    *reinterpret_cast<int*>(static_cast<uintptr_t>(slotAddr)) = a1;
    return slotAddr;
}

HOOK(0x00475850, MpSpawnPoints_RegisterObject, "auto");
