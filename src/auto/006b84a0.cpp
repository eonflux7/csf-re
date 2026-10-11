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

// Game heap free (cdecl, one pointer argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Global holding the base address of the user-data property slot table.
CSF_GLOBAL(uint32_t, g_GfxPropsSlotBase, 0x008BB488);

// Frees the block referenced by the property slot at (table base + slotOffset + 4),
// clears the slot that holds it, then clears the slot itself. Returns slotOffset.
static int __cdecl GfxObjectProps_ReleaseSlot(int slotOffset) {
    // Reload the table base at each read, as the original does.
    uint32_t tableBase = g_GfxPropsSlotBase;

    // v2 = *(uint32_t**)(base + slotOffset + 4): pointer to the block holder.
    uint32_t* blockHolder = reinterpret_cast<uint32_t*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(tableBase), slotOffset + 4));

    if (*blockHolder != 0) {
        Mem_Free(reinterpret_cast<void*>(*blockHolder));
        *blockHolder = 0;
    }

    // Re-read the table base after the free, then test the slot value.
    tableBase = g_GfxPropsSlotBase;
    uint32_t slotValue = CSF_AT(uint32_t, reinterpret_cast<void*>(tableBase), slotOffset + 4);

    if (slotValue != 0) {
        Mem_Free(reinterpret_cast<void*>(slotValue));
        tableBase = g_GfxPropsSlotBase;
        CSF_AT(uint32_t, reinterpret_cast<void*>(tableBase), slotOffset + 4) = 0;
    } else {
        CSF_AT(uint32_t, reinterpret_cast<void*>(tableBase), slotOffset + 4) = 0;
    }

    return slotOffset;
}

HOOK(0x006B84A0, GfxObjectProps_ReleaseSlot, "auto");
