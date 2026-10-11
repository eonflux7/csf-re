#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __stdcall WaypointGraph_FreeAndClearBlock(int a1)
{
    uint8_t* self = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));
    int result = CSF_AT(uint32_t, self, 0x1C);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x1C))));
        CSF_AT(uint32_t, self, 0x1C) = 0;
    }
    return result;
}

HOOK(0x75C460, WaypointGraph_FreeAndClearBlock, "auto");
