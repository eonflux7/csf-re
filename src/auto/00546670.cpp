#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap free (0x691110): __cdecl, one pointer argument, caller pops.
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* block));

// Frees the two dynamic string buffers of a menu-extras entry when they exceed
// the 15-character small-buffer capacity, then resets both to empty (capacity 15).
//   buffer at +0x2C, capacity at +0x40, length at +0x3C, first flag byte at +0x2C region
//   buffer at +0x08, capacity at +0x1C, length at +0x18, flag byte at +0x08
static int __stdcall MenuExtras_ReleaseAndResetBuffers(uint32_t* a1)
{
    int result = 0;

    if (CSF_AT(uint32_t, a1, 0x40) >= 0x10u)
        result = Mem_Free(CSF_AT(void*, a1, 0x2C));

    CSF_AT(uint32_t, a1, 0x40) = 15;
    CSF_AT(uint32_t, a1, 0x3C) = 0;
    CSF_AT(uint8_t, a1, 0x2C) = 0;

    if (CSF_AT(uint32_t, a1, 0x1C) >= 0x10u)
        result = Mem_Free(CSF_AT(void*, a1, 0x08));

    CSF_AT(uint32_t, a1, 0x18) = 0;
    CSF_AT(uint32_t, a1, 0x1C) = 15;
    CSF_AT(uint8_t, a1, 0x08) = 0;

    return result;
}

HOOK(0x00546670, MenuExtras_ReleaseAndResetBuffers, "auto");
