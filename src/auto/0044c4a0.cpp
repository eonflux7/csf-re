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

// Callee 0x44BFC0: the caller pushes four arguments and cleans up with add esp,0x10.
// Argument order (first to last): [a1+4], [a1+8], a1, a1.
CSF_FUNC(int, __cdecl, BlockRange_Release, 0x0044BFC0,
         (int a1, int a2, int a3, int a4));

// Game debug heap free used by the original (Mem_FreeDebug).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the owned pointer pair (+4 buffer, +8 secondary) and its counter (+12) of a plain
// int-array record, releasing the +4 buffer through the block-range helper first.
static int __cdecl Block_ClearOwnedPair(int* a1)
{
    int result = CSF_AT(int, a1, 0x4);
    if (result)
    {
        int secondary = CSF_AT(int, a1, 0x8);
        BlockRange_Release(CSF_AT(int, a1, 0x4), secondary,
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

HOOK(0x0044C4A0, Block_ClearOwnedPair, "auto");
