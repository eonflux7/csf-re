#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Mem_FreeDebug (game debug heap free), __cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases a mission-script parser record buffer: if the inline capacity
// (>= 0x10) has been exceeded, frees the owned heap block at +4 through the
// debug heap; then marks the buffer empty with capacity 15 and length 0.
static int __stdcall MissionScript_ReleaseRecordBuffer(int* a1)
{
    int result = 0;

    if (static_cast<uint32_t>(CSF_AT(uint32_t, a1, 0x18)) >= 0x10)
    {
        result = CSF_AT(int, a1, 0x04);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uintptr_t, a1, 0x04)));
    }

    CSF_AT(uint32_t, a1, 0x18) = 15;
    CSF_AT(uint32_t, a1, 0x14) = 0;
    CSF_AT(uint8_t, a1, 0x04) = 0;
    return result;
}

HOOK(0x469270, MissionScript_ReleaseRecordBuffer, "auto");
