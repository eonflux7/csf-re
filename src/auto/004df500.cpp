#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GrowEntryBuffer(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newBuffer = Mem_Alloc(20 * newCapacity);
    if (newBuffer &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newBuffer);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x0) = 0;
            CSF_AT(std::uint32_t, entry, 0x4) = 0;
            CSF_AT(std::uint32_t, entry, 0x8) = 0;
            CSF_AT(std::uint32_t, entry, 0xC) = 0;
            CSF_AT(std::uint32_t, entry, 0x10) = 0;
            entry += 20;
        } while (--remaining);
    }

    const std::uint32_t entryCount = CSF_AT(std::uint32_t, self, 0x8);
    if (entryCount > 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x0), 20 * entryCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldBuffer = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    int result;
    if (oldBuffer)
    {
        result = Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    else
    {
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(newBuffer));
    }

    CSF_AT(void*, self, 0x0) = newBuffer;
    return result;
}

HOOK(0x004DF500, GrowEntryBuffer, "auto");
