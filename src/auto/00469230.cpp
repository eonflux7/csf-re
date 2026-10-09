#include <cstdint>
#include "globals.h"
#include "hook.h"

// Mem_FreeDebug (game debug heap free), __cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a mission-script parser buffer: frees the owned block if the
// inline capacity (>= 0x10) is exceeded, then marks the buffer empty
// with capacity 15 and length 0.
static int __stdcall MissionScript_ResetParseBuffer(int* a1)
{
    int result = 0;

    if (static_cast<uint32_t>(CSF_AT(uint32_t, a1, 0x30)) >= 0x10)
    {
        result = CSF_AT(int, a1, 0x1C);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uintptr_t, a1, 0x1C)));
    }

    CSF_AT(uint32_t, a1, 0x30) = 15;
    CSF_AT(uint32_t, a1, 0x2C) = 0;
    CSF_AT(uint8_t, a1, 0x1C) = 0;
    return result;
}

HOOK(0x469230, MissionScript_ResetParseBuffer, "auto");
