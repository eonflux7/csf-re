#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callee: builds a collection entry for the 100-byte element at the given address.
CSF_FUNC(uint32_t*, __cdecl, sub_6B23E0, 0x006B23E0, (int a1));

// Walks the vector of 100-byte elements (begin at +8, end at +0xC) and lazily
// initializes the pointer stored at element+0x60 for each entry.
// Returns the element count (signed division of byte span by 100), or 0 when the
// begin pointer is null.
static unsigned int __fastcall GfxScene_PrepareWorldObjectCollection(uint32_t* self, void* /*edx*/)
{
    uint32_t objectIndex = 0;
    uint32_t result = 0;

    for (int byteOffset = 0; ; byteOffset += 100)
    {
        result = CSF_AT(uint32_t, self, 0x8);
        if (result == 0)
            break;

        // Signed division of the byte span by the 100-byte element size.
        int spanBytes = static_cast<int>(CSF_AT(uint32_t, self, 0xC) - result);
        result = static_cast<uint32_t>(spanBytes / 100);
        if (objectIndex >= result)
            break;

        uint32_t begin = CSF_AT(uint32_t, self, 0x8);
        uint8_t* element = reinterpret_cast<uint8_t*>(begin) + byteOffset;
        if (CSF_AT(uint32_t*, element, 0x60) == nullptr)
        {
            CSF_AT(uint32_t*, element, 0x60) =
                sub_6B23E0(static_cast<int>(reinterpret_cast<uintptr_t>(element)));
        }
        ++objectIndex;
    }
    return result;
}

HOOK(0x006CE180, GfxScene_PrepareWorldObjectCollection, "auto");
