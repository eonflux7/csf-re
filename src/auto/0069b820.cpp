#include <cstdint>
#include "globals.h"
#include "hook.h"

// Mem_FreeDebug (game debug heap free), __cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a record's owned-buffer state: frees the heap block at +8 if the
// inline capacity at +0x1C is >= 0x10, then marks the record empty with
// capacity 15, length 0 and the flag byte at +8 cleared.
static int __stdcall Record_ResetOwnedBuffer(int* a1)
{
    int result = 0;

    if (static_cast<uint32_t>(CSF_AT(uint32_t, a1, 0x1C)) >= 0x10)
    {
        result = CSF_AT(int, a1, 0x08);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uintptr_t, a1, 0x08)));
    }

    CSF_AT(uint32_t, a1, 0x1C) = 15;
    CSF_AT(uint32_t, a1, 0x18) = 0;
    CSF_AT(uint8_t, a1, 0x08) = 0;
    return result;
}

HOOK(0x69B820, Record_ResetOwnedBuffer, "auto");
