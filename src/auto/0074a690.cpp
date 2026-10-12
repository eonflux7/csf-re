#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Maps a DDL class id (stored at +0x0C) to the address of the matching
// sub-object inside the net session entry. Ids outside 0..14 return 0.
static int __cdecl NetSession_LookupEntry(int a1)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(a1);
    uint32_t classId = CSF_AT(uint32_t, entry, 0x0C);

    switch (classId)
    {
    case 0:
        return a1 - 1712;
    case 1:
        return a1 - 1088;
    case 2:
        return a1 - 776;
    case 3:
        return a1 - 216;
    case 4:
        return a1 - 340;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0xB:
        return a1 - 112;
    case 0xC:
        return a1 - 120;
    case 0xD:
        return a1 - 184;
    case 0xE:
        return a1 - 128;
    default:
        return 0;
    }
}

HOOK(0x0074A690, NetSession_LookupEntry, "auto");
