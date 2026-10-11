#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, GfxSkin_GrowEntryArray, 0x006C49B0,
         (void* self, void* edx, int a2));

static int __fastcall C_008670f0_SetEntry(
    C_008670f0* self, void* /*edx*/, int key, int value)
{
    if (!self->field_0008)
    {
        SetAllocationMode(3);
        void* storage = Mem_Alloc(16);
        if (storage)
        {
            CSF_AT(std::uint32_t, storage, 0x0) = 0;
            CSF_AT(std::uint32_t, storage, 0x4) = 0;
            CSF_AT(std::uint32_t, storage, 0x8) = 0;
            CSF_AT(std::uint32_t, storage, 0xC) = 10;
        }
        self->field_0008 = reinterpret_cast<std::uint32_t>(storage);
        SetAllocationMode(0);
    }

    void* entries = reinterpret_cast<void*>(self->field_0008);
    for (std::uint32_t index = 0;
         index < CSF_AT(std::uint32_t, entries, 0x8);
         ++index)
    {
        auto* record = static_cast<std::uint8_t*>(
            CSF_AT(void*, entries, 0x0)) + 12 * index;
        if (CSF_AT(int, record, 0x0) == key)
        {
            record = static_cast<std::uint8_t*>(
                CSF_AT(void*, entries, 0x0)) + 12 * index;
            CSF_AT(int, record, 0x4) = value;
            return static_cast<int>(3 * index);
        }
    }

    SetAllocationMode(3);
    if (CSF_AT(std::uint32_t, entries, 0x8) >=
        CSF_AT(std::uint32_t, entries, 0x4))
    {
        GfxSkin_GrowEntryArray(
            entries, nullptr, CSF_AT(int, entries, 0xC));
    }

    const std::uint32_t index = CSF_AT(std::uint32_t, entries, 0x8);
    auto* record = static_cast<std::uint8_t*>(
        CSF_AT(void*, entries, 0x0)) + 12 * index;
    CSF_AT(std::uint32_t, entries, 0x8) = index + 1;
    CSF_AT(int, record, 0x0) = key;
    CSF_AT(int, record, 0x4) = value;
    CSF_AT(int, record, 0x8) = 0;
    return SetAllocationMode(0);
}

HOOK(0x006C4B00, C_008670f0_SetEntry, "auto");
