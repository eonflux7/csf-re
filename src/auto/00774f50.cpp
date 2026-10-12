#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed
// through untouched, so it is typed as int.
CSF_FUNC(int, __fastcall, ProcessNestedEntry, 0x00774E90, (void* self, void* edx, int a2));

// Forwards a2 to sub_774E90 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns 0 instead.
static int __stdcall ForwardToNestedEntryIfNonNull(void* a1, int a2)
{
    if (a1)
        return ProcessNestedEntry(a1, nullptr, a2);
    return 0;
}

HOOK(0x00774F50, ForwardToNestedEntryIfNonNull, "auto");
