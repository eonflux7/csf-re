#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall GrowBuffer(void* self, void* /*edx*/, int extraBytes)
{
    void* newBuffer = Mem_Alloc(
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(extraBytes));

    const std::uint32_t usedBytes = CSF_AT(std::uint32_t, self, 0x8);
    if (usedBytes != 0)
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x0), usedBytes);

    void* oldBuffer = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) +=
        static_cast<std::uint32_t>(extraBytes);

    if (oldBuffer != nullptr)
    {
        const int freeResult = Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x0) = nullptr;
        CSF_AT(void*, self, 0x0) = newBuffer;
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(freeResult));
    }

    CSF_AT(void*, self, 0x0) = newBuffer;
    return newBuffer;
}

HOOK(0x0048EA50, GrowBuffer, "auto");
