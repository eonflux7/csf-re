#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int*, __cdecl, S_00470440_NormalizeBitCursor, 0x0046F010,
         (unsigned int* a1, unsigned int a2, uint32_t* a3, int a4, uint32_t* a5,
          unsigned int a6, int* a7, int* a8));

CSF_FUNC(void, __fastcall, S_00470440_Advance, 0x0046FF80,
         (void* self, void* edx, int a2));

static int* __fastcall S_00470440_ResolveBitCursor(S_00470440* self, void* /*edx*/,
                                                   int* a2, unsigned int a3, int* a4,
                                                   unsigned int a5, uint32_t* a6)
{
    // Cursor base: the stored dword pointer, advanced by whole dwords of the stored bit offset.
    uint32_t* cursorBase = reinterpret_cast<uint32_t*>(self->field_0008);
    unsigned int bitOffset = 0;
    if (self->field_0000) {
        cursorBase += self->field_0000 >> 5;
        bitOffset = self->field_0000 & 0x1F;
    }

    // Initial absolute bit index: a3 + 32 * (dword distance from the base to a4).
    int dwordDistance = static_cast<int>((uintptr_t)a4 - (uintptr_t)self->field_0008) >> 2;
    unsigned int absoluteBits = a3 + (static_cast<unsigned int>(dwordDistance) << 5);

    // Normalizes the range; a3 is updated in place by the callee.
    S_00470440_NormalizeBitCursor(reinterpret_cast<unsigned int*>(&a3), a5, a6, static_cast<int>(bitOffset),
                                  cursorBase, a3, a4, a2);

    // Re-read the base and distance after the call (callee may have changed them).
    int dwordDistanceAfter = static_cast<int>((uintptr_t)a4 - (uintptr_t)self->field_0008) >> 2;
    int advancedBits = static_cast<int>(a3 + (static_cast<unsigned int>(dwordDistanceAfter) << 5));
    S_00470440_Advance(self, nullptr, advancedBits);

    unsigned int resultDword = self->field_0008 + 4u * (absoluteBits >> 5);
    a2[0] = static_cast<int>(absoluteBits & 0x1F);
    a2[1] = static_cast<int>(resultDword);
    return a2;
}

HOOK(0x00470440, S_00470440_ResolveBitCursor, "auto");
