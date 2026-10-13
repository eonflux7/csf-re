#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall SceneLightsObjects_AssignObjectArray(
    void* self, void* /*edx*/, const void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);
        auto* allocation = static_cast<std::uint8_t*>(
            Mem_Alloc(40u * capacity + 4u));
        std::uint8_t* replacement = nullptr;

        if (allocation)
        {
            replacement = allocation + 4;
            CSF_AT(std::uint32_t, allocation, 0x00) = capacity;

            if (static_cast<std::int32_t>(capacity - 1u) >= 0)
            {
                auto* element = replacement;
                std::uint32_t remaining = capacity;
                do
                {
                    CSF_AT(void**, element, 0x08) = CSF_VTBL(0x0082D448);
                    CSF_AT(std::int32_t, element, 0x0C) = -1;
                    CSF_AT(std::int32_t, element, 0x10) = -1;
                    CSF_AT(std::uint32_t, element, 0x14) = 0;
                    CSF_AT(std::uint32_t, element, 0x18) = 0;
                    CSF_AT(std::uint32_t, element, 0x1C) = 0;
                    CSF_AT(std::int32_t, element, 0x00) = -1;
                    CSF_AT(std::int32_t, element, 0x04) = -1;
                    CSF_AT(float, element, 0x20) = FLT_MAX;
                    CSF_AT(std::uint32_t, element, 0x24) = 0;
                    element += 40;
                } while (--remaining);
            }
        }

        CSF_AT(std::uint32_t, self, 0x04) =
            CSF_AT(std::uint32_t, source, 0x04);

        auto* oldElements = CSF_AT(std::uint8_t*, self, 0x00);
        if (oldElements)
        {
            auto* oldAllocation = oldElements - 4;
            const std::uint32_t oldCapacity =
                CSF_AT(std::uint32_t, oldAllocation, 0x00);

            if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
            {
                auto* element = oldElements + 40u * oldCapacity;
                std::uint32_t remaining = oldCapacity;
                do
                {
                    element -= 40;
                    CSF_AT(void**, element, 0x08) = CSF_VTBL(0x0082D410);
                } while (--remaining);
            }

            Mem_Free(oldAllocation);
            CSF_AT(void*, self, 0x00) = nullptr;
        }

        CSF_AT(void*, self, 0x00) = replacement;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x08);
    if (count)
    {
        std::memcpy(
            CSF_AT(void*, self, 0x00),
            CSF_AT(const void*, source, 0x00),
            40u * count);
    }

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x004F10F0, SceneLightsObjects_AssignObjectArray, "auto");
