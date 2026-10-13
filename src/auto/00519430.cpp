#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Iterates the 16-byte child entries at self+0x14 (count at self+0x1C) when the
// flag byte at self+0x0D is set, calling vtable slot 2 (offset 8) on each child
// object's pointer stored at entry+8 with a2. Returns the low byte of the count.
static char __fastcall Interface_UpdateTransitionTime(void* self, void* /*edx*/, int a2)
{
    uint8_t flagIsSet = CSF_AT(uint8_t, self, 0x0D);
    if (flagIsSet == 0)
    {
        return 0;
    }

    uint32_t entryCount = CSF_AT(uint32_t, self, 0x1C);
    uint32_t entryIndex = 0;
    if (entryCount != 0)
    {
        uint32_t byteOffset = 0;
        do
        {
            uint8_t* entryBase = CSF_AT(uint8_t*, self, 0x14);
            void* childObject = CSF_AT(void*, entryBase + byteOffset, 8);

            auto updateSlot = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, childObject, 0)[2]);
            updateSlot(childObject, nullptr, a2);

            entryCount = CSF_AT(uint32_t, self, 0x1C);
            ++entryIndex;
            byteOffset += 16;
        } while (entryIndex < entryCount);
    }

    return static_cast<char>(entryCount);
}

HOOK(0x519430, Interface_UpdateTransitionTime, "auto");
