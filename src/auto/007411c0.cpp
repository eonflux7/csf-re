#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: original callees.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CMenuMpStats_ApplyOutcome, 0x00740D50, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, SpawnPoints_ApplyOutcome, 0x007452D0, (void* self, void* edx, char a2));

// Sets the attacker outcome for an MP spawn-point stats object.
// Picks the CMenuMpStats or spawn-point handler depending on Sub_64B250's result,
// then maps the outcome code (0, 1, 2) onto the matching handler argument.
static char __fastcall MpSpawnPoints_SetAttackerOutcome(C_00858180* self, void* /*edx*/, int a2)
{
    char result;

    if (Sub_64B250(self, nullptr))
    {
        if (a2)
        {
            if (a2 == 1)
            {
                return CMenuMpStats_ApplyOutcome(self, nullptr, 0);
            }
            else
            {
                result = static_cast<char>(a2 - 2);
                if (a2 == 2)
                    return CMenuMpStats_ApplyOutcome(self, nullptr, 2);
            }
        }
        else
        {
            return CMenuMpStats_ApplyOutcome(self, nullptr, 1);
        }
    }
    else if (a2)
    {
        if (a2 == 1)
        {
            return SpawnPoints_ApplyOutcome(self, nullptr, 0);
        }
        else
        {
            result = static_cast<char>(a2 - 2);
            if (a2 == 2)
                return SpawnPoints_ApplyOutcome(self, nullptr, 2);
        }
    }
    else
    {
        return SpawnPoints_ApplyOutcome(self, nullptr, 1);
    }
    return result;
}

HOOK(0x7411C0, MpSpawnPoints_SetAttackerOutcome, "auto");
