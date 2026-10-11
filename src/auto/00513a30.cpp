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

// Releases the slot buffer owned by the save-game slot block. The caller pushes four
// arguments and cleans up with add esp,0x10.
// Argument order (first to last): [a1+4], [a1+8], a1, a1.
CSF_FUNC(int, __cdecl, MenuSaveGame_ReleaseSlotBuffer, 0x005129A0,
         (uint32_t slotBuffer, uint32_t slotCount, void* slotOwnerSubobject, void* owner));

// Game debug heap free used by the original.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the save-game slot's owned pointer pair (+4 buffer, +8 secondary) and its counter (+12),
// releasing the +4 buffer through the slot-buffer helper first.
static int __cdecl SaveGameSlot_ClearOwnedBufferPair(int* a1, int /*a2*/)
{
    int result = CSF_AT(int, a1, 0x4);
    if (result)
    {
        MenuSaveGame_ReleaseSlotBuffer(CSF_AT(uint32_t, a1, 0x4), CSF_AT(uint32_t, a1, 0x8),
                                       reinterpret_cast<void*>(a1), reinterpret_cast<void*>(a1));

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

HOOK(0x00513A30, SaveGameSlot_ClearOwnedBufferPair, "auto");
