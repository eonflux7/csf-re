#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __stdcall MissionScript_FreeAndClearHandle(int* handle)
{
    int result = *handle;
    if (*handle)
    {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(*handle)));
        *handle = 0;
    }
    return result;
}

HOOK(0x468940, MissionScript_FreeAndClearHandle, "auto");
