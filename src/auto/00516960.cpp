#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is the
// std::_String_base pointer (the save file name); it is passed through untouched.
CSF_FUNC(int, __fastcall, SaveGame_ApplyFileName, 0x005167D0, (void* self, void* edx, void* a2));

// Forwards a2 to sub_5167D0 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns 0 instead.
static int __stdcall Profiles_ForwardToSaveGameIfNonNull(S_004f21d0* a1, void* a2)
{
    if (a1)
        return SaveGame_ApplyFileName(a1, nullptr, a2);
    return 0;
}

HOOK(0x00516960, Profiles_ForwardToSaveGameIfNonNull, "auto");
