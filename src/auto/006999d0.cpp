#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));
CSF_FUNC(int, __fastcall, CoreSortedTable_PopulateEntries, 0x006997A0,
         (void* self, void* edx, int a5));

static unsigned int __fastcall C_0085e720_LoadAndPackWideText(
    C_0085e720* self, void* /*edx*/, int source)
{
    if (self->field_0008)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));

    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(std::uint32_t, self, 0x10) = 0;

    SetAllocationMode(3);
    CoreSortedTable_PopulateEntries(
        reinterpret_cast<std::uint8_t*>(self) + 0x4, nullptr, source);
    SetAllocationMode(0);

    const std::uint32_t begin = self->field_0008;
    const std::uint32_t count = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_000c - begin) >> 3)
        : 0;

    std::uint32_t totalCharacters = 0;
    for (std::uint32_t index = 0; index < count; ++index)
    {
        totalCharacters += static_cast<std::uint32_t>(GetWideTextLength(
            CSF_AT(std::uint16_t*,
                   reinterpret_cast<void*>(self->field_0008), 8 * index))) + 1;
    }

    SetAllocationMode(3);
    self->field_0058 = static_cast<std::uint32_t>(
        CoreMemory_AllocUntagged(2 * totalCharacters));
    SetAllocationMode(0);

    auto* destination = reinterpret_cast<std::uint8_t*>(self->field_0058);
    for (std::uint32_t index = 0; index < count; ++index)
    {
        const std::uint32_t byteCount = 2 * (
            static_cast<std::uint32_t>(GetWideTextLength(
                CSF_AT(std::uint16_t*,
                       reinterpret_cast<void*>(self->field_0008), 8 * index))) + 1);

        std::memcpy(
            destination,
            CSF_AT(std::uint16_t*,
                   reinterpret_cast<void*>(self->field_0008), 8 * index),
            byteCount);

        auto* oldText = CSF_AT(
            std::uint16_t*, reinterpret_cast<void*>(self->field_0008), 8 * index);
        if (oldText)
        {
            CoreMemory_Free(oldText);
            CSF_AT(std::uint16_t*,
                   reinterpret_cast<void*>(self->field_0008), 8 * index) = nullptr;
        }

        CSF_AT(std::uint16_t*,
               reinterpret_cast<void*>(self->field_0008), 8 * index) =
            reinterpret_cast<std::uint16_t*>(destination);
        destination += byteCount;
    }

    return count;
}

HOOK(0x006999D0, C_0085e720_LoadAndPackWideText, "auto");
