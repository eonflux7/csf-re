#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the mp_rules scenario table dword at (entry, bomb mode, team colour),
// or -1 when the team colour is unknown or the entry index is outside 0..9.
static int __fastcall MpRules_LookupScenarioEntry(void* self, void* /*edx*/, int entryIndex, int bombMode, int teamColour)
{
    int teamSlot;
    if (teamColour == 1)
        teamSlot = 0;
    else if (teamColour == 2)
        teamSlot = 1;
    else
        teamSlot = (teamColour == 3) ? 2 : -1;

    int result = -1;
    if (teamSlot != -1 && entryIndex >= 0 && entryIndex < 10)
    {
        int modeSlot = (bombMode == 2) ? 1 : 0;
        // Table of dwords starting at +0x40: index = entry + 10*team + 30*mode
        int dwordIndex = entryIndex + 10 * teamSlot + 30 * modeSlot + 16;
        result = CSF_AT(int, self, dwordIndex * 4);
    }
    return result;
}

HOOK(0x558340, MpRules_LookupScenarioEntry, "auto");
