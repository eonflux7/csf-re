#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Grows the 8-byte-per-entry position buffer by growBy entries, copying the
// old contents and freeing the old block. Returns the old buffer pointer, or
// the result of Mem_Free when the old buffer was released.
static const void* __fastcall Gfx2d_GrowPositionBuffer(void* self, void* /*edx*/, int growBy)
{
    int totalEntries = CSF_AT(int, self, 0x4) + growBy;
    void* newBuffer = Mem_Alloc(static_cast<std::size_t>(totalEntries) * 8);

    if (newBuffer != nullptr)
    {
        auto* words = static_cast<std::uint32_t*>(newBuffer);
        for (int i = 0; i < totalEntries; ++i)
        {
            words[i * 2] = 0;
            words[i * 2 + 1] = 0;
        }
    }

    int capacity = CSF_AT(int, self, 0x8);
    if (capacity != 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x0), static_cast<std::size_t>(capacity) * 8);
    }

    void* oldBuffer = CSF_AT(void*, self, 0x0);
    const void* result = oldBuffer;
    CSF_AT(int, self, 0x4) += growBy;

    if (oldBuffer != nullptr)
    {
        result = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(Mem_Free(oldBuffer)));
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newBuffer;
    return result;
}

HOOK(0x006D64A0, Gfx2d_GrowPositionBuffer, "auto");
