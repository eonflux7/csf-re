#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the buffer at +0x4, clears the two companion words at +0x8 and +0xC,
// and returns the free result (or 0 if there was no buffer).
static int __fastcall C_0085e7b8_ClearPendingObjects(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x4);
    CSF_AT(int, self, 0x8) = 0;
    CSF_AT(int, self, 0xC) = 0;
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
        CSF_AT(int, self, 0x4) = 0;
    }
    return result;
}

HOOK(0x004F04C0, C_0085e7b8_ClearPendingObjects, "auto");
