#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxWater_RelocatePairBlock, 0x00709A80, (int a1, int a2, int a3, int a4));

// Copies the 8-byte pairs in [source, self->field_0008) into the buffer at a1, rebases
// the block through GfxWater_RelocatePairBlock, then makes the new end the block end.
// Writes a1 through the out-pointer a4 and returns a4.
static int* __fastcall GfxWater_CopyPairsIntoBuffer(S_0070d0e0* self, void* /*edx*/, int* a4, int a1, uint32_t* arg8)
{
    uint32_t* destinationCursor = reinterpret_cast<uint32_t*>(a1);

    if (reinterpret_cast<uint32_t*>(a1) != arg8)
    {
        uint32_t* sourceCursor = arg8;
        uint32_t* sourceEnd = reinterpret_cast<uint32_t*>(self->field_0008);

        if (arg8 != sourceEnd)
        {
            do
            {
                destinationCursor[0] = sourceCursor[0];
                destinationCursor[1] = sourceCursor[1];
                sourceCursor += 2;
                destinationCursor += 2;
            } while (sourceCursor != sourceEnd);
        }

        GfxWater_RelocatePairBlock(
            (int)(uintptr_t)destinationCursor,
            (int)self->field_0008,
            (int)(uintptr_t)self,
            (int)(uintptr_t)a4);

        self->field_0008 = (uint32_t)(uintptr_t)destinationCursor;
    }

    *a4 = a1;
    return a4;
}

HOOK(0x0070D0E0, GfxWater_CopyPairsIntoBuffer, "auto");
