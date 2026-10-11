#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, MpSpawnPointsBase_RefreshState, 0x00744870, (void* self, void* edx));

static int __fastcall MpSpawnPoints_SetVictimOutcome(C_00858180* self, void* /*edx*/, int outcome)
{
    MpSpawnPointsBase_RefreshState(self, nullptr);

    int result = outcome;
    if (outcome)
    {
        result = outcome - 1;
        if (outcome == 1)
            ++CSF_AT(uint16_t, self, 0x698);
    }
    else
    {
        ++CSF_AT(uint16_t, self, 0x692);
    }
    return result;
}

HOOK(0x00740D20, MpSpawnPoints_SetVictimOutcome, "auto");
