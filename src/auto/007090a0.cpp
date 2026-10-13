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

// Callee 0x6F8810: the caller pushes four arguments and cleans up with add esp,0x10.
// Argument order (first to last): [a1+4], [a1+8], a1, a1.
// The packet's bind line lists only (begin, end); the disassembly shows four pushes, so this
// prototype follows the disassembly. See notes in the metadata.
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810,
         (int begin, int end, int a3, int a4));

// Game debug heap free used by the original (Mem_FreeDebug).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the owned pointer pair (+4 buffer, +8 secondary) and its counter (+12),
// resetting the +4 entry range first and releasing the +4 buffer.
static int __cdecl GfxScreenEffects_ClearOwnedBlockPair(int* a1)
{
    int result = CSF_AT(int, a1, 0x4);
    if (result)
    {
        int secondary = CSF_AT(int, a1, 0x8);
        GfxScreenEffects_ResetEntryRange(CSF_AT(int, a1, 0x4), secondary,
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

HOOK(0x007090A0, GfxScreenEffects_ClearOwnedBlockPair, "auto");
