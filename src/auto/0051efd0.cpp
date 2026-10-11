#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall FxParticleManager_GrowParticleArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(28 * newCapacity + 4));
    std::uint32_t* replacement = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = newCapacity;
        replacement = reinterpret_cast<std::uint32_t*>(allocation + 4);

        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            auto* particle = reinterpret_cast<std::uint8_t*>(replacement);
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(void**, particle, 0x4) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, particle, 0x8) = -1;
                CSF_AT(std::int32_t, particle, 0xC) = -1;
                CSF_AT(std::uint32_t, particle, 0x0) = 0;
                CSF_AT(std::int32_t, particle, 0x10) = -1;
                CSF_AT(float, particle, 0x14) = -1.0f;
                CSF_AT(std::uint32_t, particle, 0x18) = 0;
                particle += 28;
            } while (--remaining);
        }
    }

    std::uint32_t* result = replacement;
    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount)
    {
        std::memcpy(replacement, CSF_AT(void*, self, 0x0), 28 * usedCount);
    }

    CSF_AT(std::uint32_t, self, 0x4) +=
        static_cast<std::uint32_t>(additionalCapacity);

    auto* oldParticles = CSF_AT(std::uint8_t*, self, 0x0);
    if (oldParticles)
    {
        auto* oldAllocation = oldParticles - 4;
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldAllocation, 0x0);

        if (static_cast<std::int32_t>(oldCapacity - 1) >= 0)
        {
            auto* particle = oldParticles + 28 * oldCapacity;
            std::uint32_t remaining = oldCapacity;
            do
            {
                particle -= 28;
                CSF_AT(void**, particle, 0x4) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(Mem_Free(oldAllocation)));
        CSF_AT(void*, self, 0x0) = nullptr;
        CSF_AT(void*, self, 0x0) = replacement;
    }
    else
    {
        CSF_AT(void*, self, 0x0) = result;
    }

    return result;
}

HOOK(0x0051EFD0, FxParticleManager_GrowParticleArray, "auto");
