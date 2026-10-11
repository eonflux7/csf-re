#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall FxParticleManager_GrowInstanceCache(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* replacement = Mem_Alloc(12u * newCapacity);
    if (replacement &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(replacement);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::int32_t, entry, 0x0) = -1;
            CSF_AT(std::int32_t, entry, 0x4) = -1;
            CSF_AT(std::uint32_t, entry, 0x8) = 0;
            entry += 12;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount != 0)
    {
        std::memcpy(replacement, CSF_AT(void*, self, 0x0),
                    12u * usedCount);
    }

    void* previous = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) +=
        static_cast<std::uint32_t>(additionalCapacity);

    const void* result = previous;
    if (previous)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(
                static_cast<std::uint32_t>(Mem_Free(previous))));
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = replacement;
    return result;
}

HOOK(0x0051FC20, FxParticleManager_GrowInstanceCache, "auto");
