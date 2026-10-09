#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the buffer at +0xE8, clears the two companion words at +0xEC and +0xF0,
// and returns the free result (or 0 if there was no buffer).
static int __fastcall ReleaseBufferAndResetFields(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0xE8);
    CSF_AT(int, self, 0xEC) = 0;
    CSF_AT(int, self, 0xF0) = 0;
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
        CSF_AT(int, self, 0xE8) = 0;
    }
    return result;
}

HOOK(0x0044FD70, ReleaseBufferAndResetFields, "auto");
