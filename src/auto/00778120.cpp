#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4. The receiver is passed as the
// this pointer and a2 is forwarded unchanged.
CSF_FUNC(int, __fastcall, sub_7780A0, 0x007780A0, (void* self, void* edx, int a2));

// Forwards a2 to sub_7780A0 with a1 as the this pointer when a1 is non-null;
// otherwise nothing is called. The original returned an uninitialized eax in
// that case; this returns 0 instead.
static int __stdcall GameDataDefs_ForwardIfNonNull(C_0082d410* a1, int a2)
{
    if (a1)
        return sub_7780A0(static_cast<void*>(a1), nullptr, a2);
    return 0;
}

HOOK(0x00778120, GameDataDefs_ForwardIfNonNull, "auto");
