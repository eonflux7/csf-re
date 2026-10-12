#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_0086315c_ConstructEmitterParams, 0x00701050, (C_0086315c* self, void*));
CSF_FUNC(char, __fastcall, S_007010e0_InitializeBounds, 0x007011D0, (S_007010e0* self, void* , int parameters));

[[maybe_unused]] static const auto g_ArrayCapacityReference = CSF_VTBL(0x0041E5F0);

static std::uint32_t __fastcall C_008611d4_FindOrAddEmitterBounds(
    C_008611d4* self, void* /*edx*/, int parameters)
{
    std::uint32_t index = 0;
    if (CSF_AT(std::uint32_t, self, 0x100) != 0)
    {
        const auto source = reinterpret_cast<void*>(parameters);
        const std::uint32_t key = CSF_AT(std::uint32_t, source, 0x4);
        auto entries = CSF_AT(C_0086315c**, self, 0xF8);

        do
        {
            if (CSF_AT(std::uint32_t, *entries, 0x4) == key)
                return index;

            ++index;
            ++entries;
        } while (index < CSF_AT(std::uint32_t, self, 0x100));
    }

    auto storage = static_cast<C_0086315c*>(Mem_Alloc(48));
    C_0086315c* bounds = storage
        ? reinterpret_cast<C_0086315c*>(
            C_0086315c_ConstructEmitterParams(storage, nullptr))
        : nullptr;

    if (!S_007010e0_InitializeBounds(
            reinterpret_cast<S_007010e0*>(bounds), nullptr, parameters))
    {
        if (bounds)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, bounds, 0)[0x4 / 4])(bounds, nullptr, 1);
        }
        return static_cast<std::uint32_t>(-1);
    }

    const float extentY =
        CSF_AT(float, bounds, 0x14) - CSF_AT(float, bounds, 0x20);
    const float extentX =
        CSF_AT(float, bounds, 0x10) - CSF_AT(float, bounds, 0x1C);
    const float extent = extentX < extentY ? extentY : extentX;
    const float currentExtent = CSF_AT(float, self, 0x9C);
    CSF_AT(float, self, 0x9C) =
        currentExtent < extent ? extent : currentExtent;

    if (CSF_AT(std::uint32_t, self, 0x100) >=
        CSF_AT(std::uint32_t, self, 0xFC))
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0xF8),
            nullptr, CSF_AT(int, self, 0x104));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x100);
    auto destination = CSF_AT(C_0086315c**, self, 0xF8) + count;
    CSF_AT(std::uint32_t, self, 0x100) = count + 1;
    *destination = bounds;
    return CSF_AT(std::uint32_t, self, 0x100) - 1;
}

HOOK(0x006D80D0, C_008611d4_FindOrAddEmitterBounds, "auto");
