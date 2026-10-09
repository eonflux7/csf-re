#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: grows the bit array when the new count is larger (signature from packet).
CSF_FUNC(int*, __fastcall, BitArrayGrowRange, 0x00470A50,
         (int a1, int a2, int a3, uint32_t* a4, unsigned int a5, char a6));

// Callee: shrinks the bit array when the new count is smaller (signature from packet).
CSF_FUNC(int*, __fastcall, BitArrayShrinkRange, 0x00470440,
         (void* self, void* edx, int* a2, unsigned int a3, int* a4, unsigned int a5, uint32_t* a6));

static unsigned int __fastcall MissionScript_BitArraySetCount(S_00470440* self, void* /*edx*/,
                                                              unsigned int newCount, char fillValue)
{
    const uint32_t oldCount = self->field_0000;

    // The original references the S_00470440 vtable at this call site (no store).
    (void)CSF_VTBL(0x00470440);

    if (newCount > oldCount) {
        // Grow: start at the word containing the old end.
        // The original passes this in ecx and the bit offset in edx (also pushed as the 3rd stack arg).
        uint32_t wordBase = self->field_0008;
        uint32_t bitOffsetInWord = 0;
        if (oldCount != 0) {
            wordBase += 4 * (oldCount >> 5);
            bitOffsetInWord = oldCount & 0x1F;
        }
        return static_cast<unsigned int>(reinterpret_cast<uintptr_t>(
            BitArrayGrowRange(reinterpret_cast<int>(self),
                              static_cast<int>(bitOffsetInWord),
                              static_cast<int>(bitOffsetInWord),
                              reinterpret_cast<uint32_t*>(wordBase),
                              newCount - oldCount, fillValue)));
    }

    if (newCount < oldCount) {
        // Shrink: locate the word for the new end and the old end.
        uint32_t* oldEndWord = reinterpret_cast<uint32_t*>(self->field_0008);
        uint32_t oldEndBitOffset = 0;
        if (oldCount != 0) {
            oldEndWord += oldCount >> 5;
            oldEndBitOffset = oldCount & 0x1F;
        }
        int scratchResult = 0;
        // The original leaves &scratch in edx (lea edx,[ebp-8]) and pushes it as the first stack arg.
        return static_cast<unsigned int>(reinterpret_cast<uintptr_t>(
            BitArrayShrinkRange(self, &scratchResult, &scratchResult,
                                newCount & 0x1F,
                                reinterpret_cast<int*>(self->field_0008 + 4 * (newCount >> 5)),
                                oldEndBitOffset,
                                oldEndWord)));
    }

    return oldCount;
}

HOOK(0x00470F50, MissionScript_BitArraySetCount, "auto");
