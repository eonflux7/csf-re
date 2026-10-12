#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bound original callees (by address).
CSF_FUNC(void, __cdecl, _free, 0x007B17CC, (void* Block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the buffer held at +4 when the owner flag at +0 is set.
// If the flag at +0x28 is set, the buffer came from the CRT heap and is freed
// with _free without clearing the pointer. Otherwise it is released through the
// game heap (Mem_Free) and the pointer is cleared.
static void __fastcall ReleaseOwnedBuffer(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0))
    {
        if (CSF_AT(uint8_t, self, 0x28))
        {
            _free(CSF_AT(void*, self, 4));
        }
        else
        {
            void* buffer = CSF_AT(void*, self, 4);
            if (buffer)
            {
                Mem_Free(buffer);
                CSF_AT(void*, self, 4) = nullptr;
            }
        }
    }
}

HOOK(0x69B0C0, ReleaseOwnedBuffer, "auto");
