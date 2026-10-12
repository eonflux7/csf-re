#include <cstddef>
#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Initializes a zeroed buffer of `count` 16-byte elements.
// Layout (no class given, so raw offsets):
//   +0x00 = -1, +0x04 = 0, +0x08 = count, +0x0C = buffer pointer, +0x3C = 0
// Returns nonzero (BOOL) when the allocation succeeded.
static int __fastcall ZeroedBuffer_Init(void* self, void* /*edx*/, int count)
{
    CSF_AT(int32_t, self, 0x04) = 0;
    CSF_AT(int32_t, self, 0x3C) = 0;
    CSF_AT(int32_t, self, 0x08) = count;
    CSF_AT(int32_t, self, 0x00) = -1;

    const std::size_t byteCount = static_cast<std::size_t>(count) << 4;
    void* buffer = Mem_Alloc(byteCount);
    CSF_AT(void*, self, 0x0C) = buffer;

    if (buffer)
        std::memset(buffer, 0, byteCount);

    return CSF_AT(void*, self, 0x0C) != nullptr ? 1 : 0;
}

HOOK(0x4E0930, ZeroedBuffer_Init, "auto");
