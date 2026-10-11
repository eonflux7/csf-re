#include <cstdint>
#include "globals.h"
#include "hook.h"

// Mem_Free(void* block) at 0x691110, __cdecl.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Base pointer of the per-object property slot table.
CSF_GLOBAL(uint32_t, g_ObjectPropsSlotBase, 0x008BB488);

// Frees the heap block held in a property slot and clears the slot.
// Returns Mem_Free's result when a block was present, otherwise 0.
static int __cdecl GfxObjectProps_FreeSlotBlock(int slotOffset)
{
    int** slot = reinterpret_cast<int**>(
        static_cast<uintptr_t>(g_ObjectPropsSlotBase) + static_cast<uintptr_t>(slotOffset) + 4);
    int* block = *slot;
    int result = 0;
    if (*block != 0) {
        result = Mem_Free(reinterpret_cast<void*>(*block));
        *block = 0;
    }
    return result;
}

HOOK(0x006B8400, GfxObjectProps_FreeSlotBlock, "auto");
