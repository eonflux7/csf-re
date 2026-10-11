#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees every matrix stored in a counted array of 28-byte entries, then the array block itself.
// Array layout: [int count] at data-4, followed by `count` entries of 28 bytes each.
// Entry: +8 = RwMatrix pointer, +12 = byte flag (matrix is owned when nonzero).
// The entries are visited from last to first, as the original does.
static void __fastcall FreeMatrixEntries(std::uint32_t* self, void* /*edx*/) {
    std::uint32_t arrayData = *self;
    if (arrayData == 0) {
        return;
    }

    std::uint8_t* data = reinterpret_cast<std::uint8_t*>(arrayData);
    std::uint8_t* header = data - 4;
    int count = *reinterpret_cast<int*>(header);

    for (int i = count - 1; i >= 0; --i) {
        std::uint8_t* element = data + 28 * i;
        if (CSF_AT(std::uint8_t, element, 12)) {
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(CSF_AT(std::uint32_t, element, 8)));
        }
    }

    Mem_Free(header);
    *self = 0;
}

HOOK(0x406030, FreeMatrixEntries, "auto");
