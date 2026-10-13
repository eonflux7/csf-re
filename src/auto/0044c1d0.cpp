#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed
// through untouched, so it is typed as int to match the original prototype.
CSF_FUNC(uint32_t*, __fastcall, ProcessEntryRecord, 0x0044C010, (void* self, void* edx, int a2));

// Forwards a2 to sub_44C010 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns nullptr instead.
static uint32_t* __stdcall ProcessEntryIfNonNull(uint32_t* a1, int a2)
{
    if (a1)
        return ProcessEntryRecord(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x0044C1D0, ProcessEntryIfNonNull, "auto");
