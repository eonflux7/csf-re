#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Mem_FreeDebug: game heap free, cdecl, one argument (block pointer).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a string-like object: frees its heap buffer if it grew past the
// 16-byte inline capacity, then points it back at the inline storage.
static int __stdcall ResetInlineStringBuffer(void* self)
{
    int result = 0;

    if (CSF_AT(uint32_t, self, 0x58) >= 0x10u) {
        void* heapBuffer = CSF_AT(void*, self, 0x44);
        if (heapBuffer) {
            result = Mem_FreeDebug(heapBuffer);
        }
    }

    CSF_AT(uint32_t, self, 0x58) = 15;
    CSF_AT(int, self, 0x54) = 0;
    CSF_AT(uint8_t, self, 0x44) = 0;
    CSF_AT(void*, self, 0x10) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0044BF80, ResetInlineStringBuffer, "auto");
