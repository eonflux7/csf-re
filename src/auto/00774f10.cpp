#include <cstdint>
#include "globals.h"
#include "hook.h"

// Mem_FreeDebug (game debug heap free), __cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a string buffer to an empty state with inline capacity 15. If the
// current capacity is >= 0x10, the owned heap block is freed first.
static int __stdcall GenericBuffer_ResetToInlineEmpty(int* buffer)
{
    int result = 0;

    if (static_cast<uint32_t>(CSF_AT(uint32_t, buffer, 0x38)) >= 0x10)
    {
        result = CSF_AT(int, buffer, 0x24);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uintptr_t, buffer, 0x24)));
    }

    CSF_AT(uint32_t, buffer, 0x38) = 15;
    CSF_AT(uint32_t, buffer, 0x34) = 0;
    CSF_AT(uint8_t, buffer, 0x24) = 0;
    return result;
}

HOOK(0x774F10, GenericBuffer_ResetToInlineEmpty, "auto");
