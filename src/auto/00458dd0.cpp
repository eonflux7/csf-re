#include <cstring>

#include "globals.h"
#include "hook.h"

// Removes the 40-byte entry at a3 from a contiguous array of 40-byte entries whose
// end pointer lives at self+0x08. Entries after a3 are shifted down by one slot,
// the end pointer is moved back by one slot, *a2 receives a3, and a2 is returned.
static char** __fastcall HudStatusIcons_EraseEntry(void* self, void* /*edx*/, char** a2, char* a3)
{
    constexpr int kEntrySize = 0x28;

    uint8_t* arrayEnd = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* source = reinterpret_cast<uint8_t*>(a3) + kEntrySize;

    if (source != arrayEnd)
    {
        uint8_t* destination = reinterpret_cast<uint8_t*>(a3);
        do
        {
            std::memcpy(destination, source, kEntrySize);
            source += kEntrySize;
            destination += kEntrySize;
        } while (source != arrayEnd);
    }

    CSF_AT(uint8_t*, self, 0x08) -= kEntrySize;
    *a2 = a3;
    return a2;
}

HOOK(0x458DD0, HudStatusIcons_EraseEntry, "auto");
