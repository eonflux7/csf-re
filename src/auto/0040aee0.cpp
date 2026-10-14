#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __stdcall ReleaseBlockAtOffset0C(int a1)
{
    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    int result = CSF_AT(int, obj, 0x0C);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, obj, 0x0C))));
        CSF_AT(int, obj, 0x0C) = 0;
    }
    return result;
}

HOOK(0x40AEE0, ReleaseBlockAtOffset0C, "auto");
