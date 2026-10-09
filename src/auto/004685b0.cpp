#include <cstdint>
#include "globals.h"
#include "hook.h"

// __fastcall (this = ecx, one stack arg), ret 4. The original sets ecx to the
// non-null a1 before the call, so a1 is passed as this; a2 is the stack argument.
CSF_FUNC(uint32_t*, __fastcall, MissionScript_ProcessEntry, 0x004419F0, (void* self, void* edx, uint32_t* a2));

// Forwards a2 to sub_4419F0 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns 0 instead.
static int __stdcall MissionScript_ForwardEntryIfNonNull(int a1, int a2)
{
    if (a1)
        return static_cast<int>(reinterpret_cast<intptr_t>(
            MissionScript_ProcessEntry(reinterpret_cast<void*>(static_cast<intptr_t>(a1)),
                                       nullptr,
                                       reinterpret_cast<uint32_t*>(static_cast<intptr_t>(a2)))));
    return 0;
}

HOOK(0x004685B0, MissionScript_ForwardEntryIfNonNull, "auto");
