#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the two heap buffers owned by the save-game slot record, then resets
// the record to its empty state (length/capacity fields and the 16-bit flag word).
static int __fastcall ProfilesSaveGames_ResetBuffers(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x38);
    if (result != 0)
        result = Mem_FreeDebug(CSF_AT(void*, self, 0x38));

    CSF_AT(void*, self, 0x38) = nullptr;
    CSF_AT(void*, self, 0x3C) = nullptr;
    CSF_AT(void*, self, 0x40) = nullptr;

    if (CSF_AT(uint32_t, self, 0x18) >= 8u)
    {
        result = CSF_AT(int, self, 0x04);
        if (result != 0)
            result = Mem_FreeDebug(CSF_AT(void*, self, 0x04));
    }

    CSF_AT(void*, self, 0x14) = nullptr;
    CSF_AT(uint32_t, self, 0x18) = 7u;
    CSF_AT(uint16_t, self, 0x04) = 0;
    return result;
}

HOOK(0x514AC0, ProfilesSaveGames_ResetBuffers, "auto");
