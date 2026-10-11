#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, GfxSkin_GrowKeyedValues, 0x006C49B0,
         (void* self, void* edx, int a2));

static int __fastcall GfxSkin_SetKeyedValue(
    void* self, void* /*edx*/, int key, unsigned int value)
{
    if (!CSF_AT(void*, self, 0x8))
    {
        SetAllocationMode(3);
        void* storage = Mem_Alloc(16);
        if (storage)
        {
            CSF_AT(std::uint32_t, storage, 0x0) = 0;
            CSF_AT(std::uint32_t, storage, 0x4) = 0;
            CSF_AT(std::uint32_t, storage, 0x8) = 0;
            CSF_AT(int, storage, 0xC) = 10;
        }
        CSF_AT(void*, self, 0x8) = storage;
        SetAllocationMode(0);
    }

    void* storage = CSF_AT(void*, self, 0x8);
    const std::uint32_t taggedKey =
        static_cast<std::uint32_t>(key) | 0x80000000;

    if (CSF_AT(std::uint32_t, storage, 0x8))
    {
        std::uint32_t index = 0;
        do
        {
            auto* entries = CSF_AT(std::uint8_t*, storage, 0x0);
            if (CSF_AT(std::uint32_t, entries + 12 * index, 0x0) == taggedKey)
            {
                entries = CSF_AT(std::uint8_t*, storage, 0x0);
                CSF_AT(std::uint32_t, entries + 12 * index, 0x4) = value;
                return static_cast<int>(
                    reinterpret_cast<std::uintptr_t>(entries));
            }
            ++index;
        }
        while (index < CSF_AT(std::uint32_t, storage, 0x8));
    }

    SetAllocationMode(3);
    if (CSF_AT(std::uint32_t, storage, 0x8) >=
        CSF_AT(std::uint32_t, storage, 0x4))
    {
        GfxSkin_GrowKeyedValues(
            storage, nullptr, CSF_AT(int, storage, 0xC));
    }

    const std::uint32_t index = CSF_AT(std::uint32_t, storage, 0x8);
    auto* entries = CSF_AT(std::uint8_t*, storage, 0x0);
    CSF_AT(std::uint32_t, storage, 0x8) = index + 1;

    auto* entry = entries + 12 * index;
    CSF_AT(std::uint32_t, entry, 0x0) = taggedKey;
    CSF_AT(std::uint32_t, entry, 0x4) = value;
    CSF_AT(std::uint32_t, entry, 0x8) = 0;
    return SetAllocationMode(0);
}

HOOK(0x006C4BF0, GfxSkin_SetKeyedValue, "auto");
