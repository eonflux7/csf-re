#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Walks the circular list headed at self->field_002C and returns the payload of the
// first entry whose (+8, +12, +16 dwords and +21 byte) match the four arguments, or 0.
static int __fastcall GfxRenderTargets_FindMatchingEntry(void* self, void* /*edx*/,
                                                         int a2, int a3, int a4, char a5)
{
    uint8_t* listHead = CSF_AT(uint8_t*, self, 0x2c);
    uint8_t* node = CSF_AT(uint8_t*, listHead, 0);
    uintptr_t found = 0;

    for (; node != listHead; node = CSF_AT(uint8_t*, node, 0)) {
        if (found)
            break;

        uint8_t* entry = CSF_AT(uint8_t*, node, 8);
        if (CSF_AT(int, entry, 8) == a2 &&
            CSF_AT(int, entry, 12) == a3 &&
            CSF_AT(int, entry, 16) == a4 &&
            CSF_AT(char, entry, 21) == a5)
        {
            found = reinterpret_cast<uintptr_t>(entry);
        }
    }
    return static_cast<int>(found);
}

HOOK(0x6D0920, GfxRenderTargets_FindMatchingEntry, "auto");
