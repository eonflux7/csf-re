#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall-style callee (this = ecx, one stack arg), ret 4. The original pushes
// a2 and loads a1 into ecx, so a1 is the this pointer and a2 is the stack argument.
CSF_FUNC(char*, __fastcall, MissionScript_ParseEntry, 0x00469FD0, (void* self, void* edx, int a2));

// Forwards a2 to MissionScript_ParseEntry (with a1 as the this pointer) when a1 is
// non-null; otherwise nothing is done. The original returned an uninitialized eax in
// that case; this returns nullptr instead.
static char* __stdcall MissionScript_ParseEntryIfNonNull(char* a1, int a2)
{
    if (a1)
        return MissionScript_ParseEntry(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x0046A0E0, MissionScript_ParseEntryIfNonNull, "auto");
