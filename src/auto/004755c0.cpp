#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Reads the buffer pointer at +0x28, clears the two companion words at +0x2C and +0x30,
// then frees the buffer (if any) and clears the pointer at +0x28.
// Returns the free result, or 0 if there was no buffer.
static int __fastcall ReleaseBufferAndClearCounters(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x28);
    CSF_AT(int, self, 0x2C) = 0;
    CSF_AT(int, self, 0x30) = 0;
    if (result) {
        result = CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
        CSF_AT(int, self, 0x28) = 0;
    }
    return result;
}

HOOK(0x004755C0, ReleaseBufferAndClearCounters, "auto");
