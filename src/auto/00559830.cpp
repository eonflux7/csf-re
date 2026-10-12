#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: releases the bomb data sub-block (unnamed in the packet; name chosen here).
CSF_FUNC(int, __cdecl, MpRules_CleanupBombDataBlock, 0x00558DA0, (int a1, int a2));

// Game heap free with debug info.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the bomb data block held in a sabotage-bomb record and clears its slots.
// a1[2] = data block pointer, a1[3] = secondary value passed to cleanup,
// a1[4] = trailing slot. The record's slots start at a1 + 1 (esi in the original).
static int __stdcall MpRules_ReleaseBombDataBlock(int* a1)
{
    int* slots = a1 + 1;   // esi = a1 + 4 bytes
    int result = slots[1]; // a1[2]

    if (result)
    {
        MpRules_CleanupBombDataBlock(result, slots[2]);

        // The original reloads the block pointer after the cleanup call.
        result = slots[1];
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(slots[1]));
    }

    slots[1] = 0;
    slots[2] = 0;
    slots[3] = 0;
    return result;
}

HOOK(0x00559830, MpRules_ReleaseBombDataBlock, "auto");
