#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(uint32_t, dword_8BB484, 0x008BB484);
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the block pointed to by the table slot at (table base + slotIndex + 4),
// marks its first dword as 3, and clears the slot. Returns slotIndex.
static int __cdecl ReleasePropertySlot(int slotIndex)
{
    uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(dword_8BB484));
    uint32_t blockAddr = CSF_AT(uint32_t, table, slotIndex + 4);

    if (blockAddr != 0)
    {
        *reinterpret_cast<uint32_t*>(blockAddr) = 3;
        Mem_Free(reinterpret_cast<void*>(blockAddr));

        table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(dword_8BB484));
        CSF_AT(uint32_t, table, slotIndex + 4) = 0;
    }
    else
    {
        CSF_AT(uint32_t, table, slotIndex + 4) = 0;
    }

    return slotIndex;
}

HOOK(0x006B9060, ReleasePropertySlot, "auto");
