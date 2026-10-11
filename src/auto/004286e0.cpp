#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Cheats (truco) table: 31 entries of 0x3C bytes, each holding up to three
// (key1, key2, slot) groups. Entries are scanned from this+4; whenever a group's
// key pair equals (a2, a3) its slot is set to 3. Returns the one-past-the-end
// cursor (this + 0x750), as the original leaves it in eax.
static uint32_t* __fastcall Cheats_MarkEntriesMatchingPair(void* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* base = static_cast<uint8_t*>(self);
    uint8_t* entry = base + 0x4;
    int remaining = 31;

    do
    {
        if (CSF_AT(int, entry, 0x0) == a2 && CSF_AT(int, entry, 0x4) == a3)
            CSF_AT(uint32_t, entry, 0x8) = 3;

        if (CSF_AT(int, entry, 0x14) == a2 && CSF_AT(int, entry, 0x18) == a3)
            CSF_AT(uint32_t, entry, 0x1C) = 3;

        if (CSF_AT(int, entry, 0x28) == a2 && CSF_AT(int, entry, 0x2C) == a3)
            CSF_AT(uint32_t, entry, 0x30) = 3;

        entry += 0x3C;
        --remaining;
    } while (remaining != 0);

    return reinterpret_cast<uint32_t*>(entry + 0x8);
}

HOOK(0x4286E0, Cheats_MarkEntriesMatchingPair, "auto");
