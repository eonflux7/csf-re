#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The result is returned in eax.
CSF_FUNC(int*, __fastcall, Entry_ProcessWithArg, 0x006B5260, (void* self, void* edx, int a2));

// Forwards a2 to sub_6B5260 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns nullptr instead.
static int* __stdcall ForwardToEntryIfNonNull(void* a1, int a2)
{
    if (a1)
        return Entry_ProcessWithArg(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x006B5510, ForwardToEntryIfNonNull, "auto");
