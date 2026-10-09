#include <cstdint>
#include "globals.h"
#include "hook.h"

// Resolves an entry's lookup by forwarding to the routine stored at self+0x6C,
// using the signed 16-bit field at entry+2 plus the caller's base index.
CSF_FUNC(int, __fastcall, MissionScript_LookupRoutineAt46F6C0, 0x0046F6C0, (void* self, void* edx, unsigned int a2));

static int __fastcall MissionScript_ResolveEntryIndexViaSelf(void* self, void* /*edx*/, void* entry, int baseIndex)
{
    void* lookupRoutineOwner = CSF_AT(void*, self, 0x6C);
    int16_t entryOffset = CSF_AT(int16_t, entry, 2);
    return MissionScript_LookupRoutineAt46F6C0(lookupRoutineOwner, nullptr, static_cast<unsigned int>(baseIndex + entryOffset));
}

HOOK(0x0046F970, MissionScript_ResolveEntryIndexViaSelf, "auto");
