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
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* replacement = Mem_Alloc(newCapacity * 32);
    if (replacement &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(replacement);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            CSF_AT(std::int32_t, entry, 0x00) = -1;
            CSF_AT(float, entry, 0x14) = 1.0f;
            CSF_AT(std::uint32_t, entry, 0x10) = 0;
            CSF_AT(std::uint32_t, entry, 0x18) = 0;
            CSF_AT(std::uint32_t, entry, 0x1C) = 0;
            entry += 32;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(replacement, CSF_AT(void*, self, 0x00), usedCount * 32);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldBuffer = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    int result;
    if (oldBuffer)
    {
        result = Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x00) = nullptr;
    }
    else
    {
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(replacement));
    }

    CSF_AT(void*, self, 0x00) = replacement;
    return result;
}

HOOK(0x00445A80, GrowEntryBuffer, "auto");
