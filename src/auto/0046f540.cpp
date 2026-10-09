#include <cstdint>
#include "globals.h"
#include "hook.h"

// Resolves an entry by index: forwards to the lookup routine stored at self+0x6C,
// using the signed 16-bit field at entry+2 plus the caller's base index.
CSF_FUNC(int, __fastcall, MissionScript_LookupByIndex, 0x0046F4D0, (void* self, void* edx, int a2));

static int __fastcall MissionScript_ForwardLookupAtEntryOffset(void* self, void* /*edx*/, void* entry, int baseIndex)
{
    void* lookupTable = CSF_AT(void*, self, 0x6C);
    int16_t entryOffset = CSF_AT(int16_t, entry, 2);
    return MissionScript_LookupByIndex(lookupTable, nullptr, baseIndex + entryOffset);
}

HOOK(0x0046F540, MissionScript_ForwardLookupAtEntryOffset, "auto");
