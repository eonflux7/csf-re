#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the three owned blocks of a sound object:
//   +0x20: plain heap block, freed and cleared.
//   +0x10: array of 24-byte elements with an element count stored 4 bytes before the
//          data pointer. Each element's dword at +8 is overwritten with the vtable
//          0x82D410, then the whole allocation (data - 4) is freed.
//   +0x00: heap block, freed and cleared; its Mem_Free result is returned.
static int __fastcall SoundManager_ReleaseOwnedBlocks(void* self, void* /*edx*/)
{
    if (CSF_AT(void*, self, 0x20)) {
        Mem_Free(CSF_AT(void*, self, 0x20));
        CSF_AT(void*, self, 0x20) = nullptr;
    }

    void* table = CSF_AT(void*, self, 0x10);
    if (table) {
        uint8_t* elements = static_cast<uint8_t*>(table);
        int32_t elementCount = CSF_AT(int32_t, elements, -4);

        // Walk the elements from the last one down to the first.
        for (int32_t i = elementCount - 1; i >= 0; --i) {
            CSF_AT(void*, elements + 24 * i, 8) = CSF_VTBL(0x0082D410);
        }

        Mem_Free(elements - 4);
        CSF_AT(void*, self, 0x10) = nullptr;
    }

    int result = CSF_AT(int, self, 0);
    if (CSF_AT(void*, self, 0)) {
        result = Mem_Free(CSF_AT(void*, self, 0));
        CSF_AT(void*, self, 0) = nullptr;
    }
    return result;
}

HOOK(0x00522DB0, SoundManager_ReleaseOwnedBlocks, "auto");
