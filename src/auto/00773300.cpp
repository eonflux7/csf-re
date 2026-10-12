#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is an
// animation-entry parameter that is passed through untouched.
CSF_FUNC(char*, __fastcall, AnimDefs_ProcessAnimEntry, 0x00773160, (void* self, void* edx, int a2));

// Forwards a2 to sub_773160 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns nullptr instead.
static char* __stdcall AnimDefs_ForwardToEntryIfNonNull(char* a1, int a2)
{
    if (a1)
        return AnimDefs_ProcessAnimEntry(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x00773300, AnimDefs_ForwardToEntryIfNonNull, "auto");
