#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed
// through untouched as an integer value.
CSF_FUNC(char*, __fastcall, sub_770E80, 0x00770E80, (void* self, void* edx, int a2));

// Forwards a2 to sub_770E80 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns nullptr instead.
static char* __stdcall SoundAreas_ForwardToEntryIfNonNull(char* a1, uint32_t* a2)
{
    if (a1)
        return sub_770E80(a1, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(a2)));
    return nullptr;
}

HOOK(0x00770F80, SoundAreas_ForwardToEntryIfNonNull, "auto");
