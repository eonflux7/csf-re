#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed
// through untouched, so it is typed as void*.
CSF_FUNC(int, __fastcall, Entry_ProcessWithThis, 0x007828A0, (void* self, void* edx, void* a2));

// Forwards a2 to sub_7828A0 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns 0 instead.
static int __stdcall Entry_ForwardToProcessIfNonNull(C_0082d410* a1, C_0082d410* a2)
{
    if (a1)
        return Entry_ProcessWithThis(static_cast<void*>(a1), nullptr, static_cast<void*>(a2));
    return 0;
}

HOOK(0x00782910, Entry_ForwardToProcessIfNonNull, "auto");
