#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed through untouched.
CSF_FUNC(char*, __fastcall, CampaignEntry_Process, 0x0043ECA0, (void* self, void* edx, int a2));

// Forwards a2 to sub_43ECA0 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns nullptr instead.
static char* __stdcall Campaign_ProcessEntryIfNonNull(char* a1, int a2)
{
    if (a1)
        return CampaignEntry_Process(reinterpret_cast<void*>(a1), nullptr, a2);
    return nullptr;
}

HOOK(0x0043EE40, Campaign_ProcessEntryIfNonNull, "auto");
