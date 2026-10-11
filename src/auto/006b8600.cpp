#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Base address of the table that holds the per-slot object pointers. The
// table is indexed by byte offsets (slot * stride), so the global is a byte base.
CSF_GLOBAL(uint8_t*, g_PropertySlotTableBase, 0x008BB484);

// Copies the value of the property slot at `sourceSlotOffset` into the slot at
// `destSlotOffset`, after writing the tag 3 into the destination first.
// Both arguments are byte offsets into the slot table. Returns the destination offset.
static int __cdecl GfxObjectProps_CopyPropertySlot(int destSlotOffset, int sourceSlotOffset) {
    uint32_t* sourceObject =
        *reinterpret_cast<uint32_t**>(g_PropertySlotTableBase + sourceSlotOffset + 4);
    uint32_t* destObject =
        *reinterpret_cast<uint32_t**>(g_PropertySlotTableBase + destSlotOffset + 4);

    *destObject = 3;
    *destObject = *sourceObject;
    return destSlotOffset;
}

HOOK(0x006B8600, GfxObjectProps_CopyPropertySlot, "auto");
