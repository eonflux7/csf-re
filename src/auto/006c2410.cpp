#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Walks the 8 slots that start at +0x04 of the object. Each slot holds a pointer to a
// cell; if that cell holds a non-null block it is freed and the cell is cleared, then
// the slot's own block is freed and the slot is cleared. Returns the low byte of the
// last slot's CoreMemory_Free result (0 if that slot was empty).
static char __fastcall Keyframe_ReleaseAllSlots(S_006c20f0* self, void* /*edx*/)
{
    char lastResult = 0;

    for (int slotIndex = 0; slotIndex < 8; ++slotIndex)
    {
        uint32_t slotOffset = 4 + static_cast<uint32_t>(slotIndex) * 4;
        uint32_t cellPtr = CSF_AT(uint32_t, self, slotOffset);
        lastResult = 0;

        if (cellPtr)
        {
            uint32_t innerBlock = *reinterpret_cast<uint32_t*>(cellPtr);
            if (innerBlock)
            {
                CoreMemory_Free(reinterpret_cast<void*>(innerBlock));
                *reinterpret_cast<uint32_t*>(cellPtr) = 0;
            }

            cellPtr = CSF_AT(uint32_t, self, slotOffset);
            if (cellPtr)
            {
                lastResult = static_cast<char>(CoreMemory_Free(reinterpret_cast<void*>(cellPtr)));
                CSF_AT(uint32_t, self, slotOffset) = 0;
            }
        }
    }

    return lastResult;
}

HOOK(0x006C2410, Keyframe_ReleaseAllSlots, "auto");
