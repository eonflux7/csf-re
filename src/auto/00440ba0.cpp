#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee 0x43EDB0: the caller pushes four arguments and cleans up with add esp,0x10.
// Argument order (first to last): [a3+4], [a3+8], a3, a3.
CSF_FUNC(int, __cdecl, CampaignBriefing_DestroyBlockRange, 0x0043EDB0,
         (int a1, int a2, int a3, int a4));

// Game debug heap free used by the original (Mem_FreeDebug).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the briefing script's owned pointer pair (+4 buffer, +8 secondary) and its counter (+12),
// releasing the +4 buffer through the block-range helper first.
static int __cdecl CampaignBriefing_ClearOwnedBlockPair(int* a1)
{
    int result = CSF_AT(int, a1, 0x4);
    if (result)
    {
        int secondary = CSF_AT(int, a1, 0x8);
        CampaignBriefing_DestroyBlockRange(CSF_AT(int, a1, 0x4), secondary,
                                           reinterpret_cast<int>(a1), reinterpret_cast<int>(a1));

        result = CSF_AT(int, a1, 0x4);
        if (result)
        {
            result = Mem_FreeDebug(CSF_AT(void*, a1, 0x4));
        }
    }

    CSF_AT(uint32_t, a1, 0x4) = 0;
    CSF_AT(uint32_t, a1, 0x8) = 0;
    CSF_AT(uint32_t, a1, 0xC) = 0;
    return result;
}

HOOK(0x00440BA0, CampaignBriefing_ClearOwnedBlockPair, "auto");
