#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x0043EDB0 is __cdecl with four int arguments.
CSF_FUNC(int, __cdecl, Briefing_ProcessSoundEntry, 0x0043EDB0,
         (int a1, int a2, int a3, int a4));

// 0x0043F840: __fastcall-style thunk with two stack arguments (ret 8).
// The original overwrites ECX with the second stack argument before pushing,
// so the incoming ECX is never read. The callee receives
// (arg0, arg1, arg1, arg1).
static int __fastcall CampaignBriefing_ForwardSoundArguments(
    void* /*context*/, void* /*edx*/, int firstArgument, int secondArgument)
{
    return Briefing_ProcessSoundEntry(
        firstArgument,
        secondArgument,
        secondArgument,
        secondArgument);
}

HOOK(0x0043F840, CampaignBriefing_ForwardSoundArguments, "auto");
