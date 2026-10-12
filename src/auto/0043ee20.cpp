#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4. Bound by address; the second
// argument is passed through untouched.
CSF_FUNC(int, __fastcall, Campaign_ProcessEntry, 0x0043EC50, (void* self, void* edx, int a2));

// Forwards a2 to Campaign_ProcessEntry (with a1 as the this pointer) when a1 is
// non-null; otherwise nothing is done. The original returned an uninitialized eax
// in that case; this returns 0 instead.
static int __stdcall Campaign_ForwardToEntryIfNonNull(C_0082d410* a1, int a2)
{
    if (a1)
        return Campaign_ProcessEntry(a1, nullptr, a2);
    return 0;
}

HOOK(0x0043EE20, Campaign_ForwardToEntryIfNonNull, "auto");
