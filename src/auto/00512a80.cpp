#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is the
// std::_String_base pointer; it is passed through untouched, so it is typed as void*.
CSF_FUNC(int, __fastcall, SaveGames_ProcessEntry, 0x00512820, (void* self, void* edx, void* a2));

// Forwards a2 to sub_512820 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns 0 instead.
static int __stdcall SaveGames_ForwardToEntryIfNonNull(void* a1, void* a2)
{
    if (a1)
        return SaveGames_ProcessEntry(a1, nullptr, a2);
    return 0;
}

HOOK(0x00512A80, SaveGames_ForwardToEntryIfNonNull, "auto");
