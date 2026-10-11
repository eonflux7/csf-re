#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. Resolves an entry from the
// character/vehicle data table for the given key; the result is returned as-is.
CSF_FUNC(char*, __fastcall, CharacterDefs_ResolveEntry, 0x0054DA80, (void* self, void* edx, int a2));

// Forwards a2 to CharacterDefs_ResolveEntry (with a1 as the this pointer) when
// a1 is non-null; otherwise nothing is done. The original returned an
// uninitialized eax in that case; this returns nullptr instead.
static char* __stdcall CharacterDefs_ResolveEntryIfNonNull(int a1, int a2)
{
    if (a1)
        return CharacterDefs_ResolveEntry(reinterpret_cast<void*>(static_cast<intptr_t>(a1)), nullptr, a2);
    return nullptr;
}

HOOK(0x00762CB0, CharacterDefs_ResolveEntryIfNonNull, "auto");
