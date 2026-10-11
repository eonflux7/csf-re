#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall int-arg accessor: returns a pointer to the 36-byte entry at
// index a2 of the array stored at this+4, and ORs 0x200 into its dword at +20.
static uint32_t* __fastcall C_00861e98_SetEntryFlag0x200(C_00861e98* self, void* /*edx*/, int entryIndex)
{
    uint8_t* entryArrayBase = CSF_AT(uint8_t*, self, 0x04);
    uint32_t* flagWord = reinterpret_cast<uint32_t*>(entryArrayBase + 36 * entryIndex + 20);
    *flagWord |= 0x200u;
    return flagWord;
}

HOOK(0x006E9EB0, C_00861e98_SetEntryFlag0x200, "auto");
