#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, HudStringShow_DestroyBufferRange, 0x004F23A0, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall HudStringShow_ReleaseBuffer(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x04);
    if (result)
    {
        HudStringShow_DestroyBufferRange(result, CSF_AT(int, self, 0x08));
        result = CSF_AT(int, self, 0x04);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x004F2B70, HudStringShow_ReleaseBuffer, "auto");
