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

// Mem_FreeDebug: __cdecl, takes one block pointer, returns int.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a spawn-point record: releases its buffer when the capacity field is >= 8,
// then sets capacity to 7, count to 0 and clears the low word of the buffer pointer.
static int __fastcall SpawnPoints_ResetRecord(void* self, void* /*edx*/)
{
    int result = 0;

    if (static_cast<uint32_t>(CSF_AT(int, self, 0x20)) >= 8)
    {
        uint32_t buffer = CSF_AT(uint32_t, self, 0x0C);
        if (buffer != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(buffer));
        }
    }

    CSF_AT(int, self, 0x20) = 7;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(uint16_t, self, 0x0C) = 0;
    return result;
}

HOOK(0x554690, SpawnPoints_ResetRecord, "auto");
