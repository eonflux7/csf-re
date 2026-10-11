#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall FxParticleManager_AssignParticleArray(
    std::uint32_t* self, void* /*edx*/, const std::uint32_t* source)
{
    if (CSF_AT(std::uint32_t, source, 0x8) >
        CSF_AT(std::uint32_t, self, 0x4))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x4);
        auto* allocation = static_cast<std::uint8_t*>(
            Mem_Alloc(28u * capacity + 4u));
        std::uint8_t* newElements = nullptr;

        if (allocation)
        {
            CSF_AT(std::uint32_t, allocation, 0x0) = capacity;
            newElements = allocation + 4;

            if (static_cast<std::int32_t>(capacity - 1u) >= 0)
            {
                std::uint8_t* element = newElements;
                std::uint32_t remaining = capacity;
                do
                {
                    CSF_AT(void**, element, 0x4) = CSF_VTBL(0x0082D448);
                    CSF_AT(std::int32_t, element, 0x8) = -1;
                    CSF_AT(std::int32_t, element, 0xC) = -1;
                    CSF_AT(std::uint32_t, element, 0x0) = 0;
                    CSF_AT(std::int32_t, element, 0x10) = -1;
                    CSF_AT(float, element, 0x14) = -1.0f;
                    CSF_AT(std::uint32_t, element, 0x18) = 0;
                    element += 28;
                } while (--remaining);
            }
        }

        CSF_AT(std::uint32_t, self, 0x4) =
            CSF_AT(std::uint32_t, source, 0x4);

        auto* oldElements = CSF_AT(std::uint8_t*, self, 0x0);
        if (oldElements)
        {
            auto* oldAllocation = oldElements - 4;
            const std::uint32_t oldCapacity =
                CSF_AT(std::uint32_t, oldAllocation, 0x0);

            if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
            {
                std::uint32_t remaining = oldCapacity;
                do
                {
                    --remaining;
                    CSF_AT(void**, oldElements + 28u * remaining, 0x4) =
                        CSF_VTBL(0x0082D410);
                } while (remaining);
            }

            Mem_Free(oldAllocation);
            CSF_AT(std::uint8_t*, self, 0x0) = nullptr;
        }
        CSF_AT(std::uint8_t*, self, 0x0) = newElements;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x8);
    if (count)
    {
        const void* sourceElements = CSF_AT(const void*, source, 0x0);
        void* destinationElements = CSF_AT(void*, self, 0x0);
        std::memcpy(destinationElements, sourceElements, 28u * count);
    }

    CSF_AT(std::uint32_t, self, 0x8) =
        CSF_AT(std::uint32_t, source, 0x8);
    CSF_AT(std::uint32_t, self, 0xC) =
        CSF_AT(std::uint32_t, source, 0xC);
    return self;
}

HOOK(0x0051EEC0, FxParticleManager_AssignParticleArray, "auto");
