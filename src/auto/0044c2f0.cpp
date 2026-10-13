#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, DestroyBufferElements, 0x0044BFC0,
         (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall ReleaseIndexedBuffer(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x04);
    if (result != 0)
    {
        DestroyBufferElements(result, CSF_AT(int, self, 0x08),
                              reinterpret_cast<int>(self), 0);
        result = CSF_AT(int, self, 0x04);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x0044C2F0, ReleaseIndexedBuffer, "auto");
