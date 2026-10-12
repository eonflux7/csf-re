#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static const auto g_ArrayCapacityVtable = CSF_VTBL(0x0041E5F0);

static int __fastcall GfxLights_UpdateTypeMembership(
    void* self, void* /*edx*/, int object, int oldType, int newType)
{
    int result = CSF_AT(int, self, 0x84);
    if (result == object)
        return result;

    result = 3;
    if (oldType == 3)
    {
        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x5C);
        std::uint32_t index = static_cast<std::uint32_t>(-1);

        if (count != 0)
        {
            const auto* entries = reinterpret_cast<const int*>(
                CSF_AT(std::uint32_t, self, 0x54));

            for (std::uint32_t i = 0; i < count; ++i)
            {
                if (entries[i] == object)
                {
                    index = i;
                    break;
                }
            }
        }

        const std::uint32_t newCount =
            CSF_AT(std::uint32_t, self, 0x5C) - 1;
        CSF_AT(std::uint32_t, self, 0x5C) = newCount;
        result = static_cast<int>(newCount);

        if (index != newCount)
        {
            const std::uint32_t destinationAddress =
                CSF_AT(std::uint32_t, self, 0x54) + 4 * index;
            void* destination = reinterpret_cast<void*>(destinationAddress);
            const void* source =
                reinterpret_cast<const void*>(destinationAddress + 4);

            return static_cast<int>(reinterpret_cast<std::uintptr_t>(
                _memcpy(destination, source, 4 * (newCount - index))));
        }
    }
    else if (newType == 3)
    {
        auto* array = reinterpret_cast<S_0041e5f0*>(
            static_cast<std::uint8_t*>(self) + 0x54);

        if (CSF_AT(std::uint32_t, self, 0x5C) >=
            CSF_AT(std::uint32_t, self, 0x58))
        {
            S_0041e5f0_GrowArrayCapacity(
                array, nullptr, CSF_AT(int, self, 0x60));
        }

        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x5C);
        auto* entry = reinterpret_cast<int*>(
            CSF_AT(std::uint32_t, self, 0x54) + 4 * count);

        const std::uint32_t newCount = count + 1;
        CSF_AT(std::uint32_t, self, 0x5C) = newCount;
        *entry = object;
        result = static_cast<int>(newCount);
    }

    return result;
}

HOOK(0x006D2030, GfxLights_UpdateTypeMembership, "auto");
