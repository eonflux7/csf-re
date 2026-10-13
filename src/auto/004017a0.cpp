#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static std::uint32_t __fastcall Table20_GrowEntryArray(
    S_004017a0* self, void* /*edx*/, int a2)
{
    const std::uint32_t newCount = self->field_0004 + static_cast<std::uint32_t>(a2);

    auto* allocation = reinterpret_cast<std::uint8_t*>(
        CoreMemory_AllocUntagged(20u * newCount + 4u));
    std::uint8_t* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = newCount;
        newEntries = allocation + 4;

        if (static_cast<std::int32_t>(newCount - 1u) >= 0)
        {
            std::uint8_t* entry = newEntries;
            std::uint32_t remaining = newCount;
            do
            {
                CSF_AT(void**, entry, 0x4) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x8) = -1;
                CSF_AT(std::int32_t, entry, 0xC) = -1;
                entry += 20;
            } while (--remaining);
        }
    }

    std::uint32_t result = self->field_0008;
    if (result != 0)
    {
        result *= 20u;
        std::memcpy(newEntries,
            reinterpret_cast<const void*>(static_cast<std::uintptr_t>(self->field_0000)),
            result);
    }

    const std::uint32_t oldEntriesAddr = self->field_0000;
    self->field_0004 += static_cast<std::uint32_t>(a2);

    if (oldEntriesAddr != 0)
    {
        std::uint8_t* oldEntries =
            reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(oldEntriesAddr));
        const std::uint32_t oldCount = CSF_AT(std::uint32_t, oldEntries, -4);
        std::uint8_t* entry = oldEntries + 20u * oldCount;

        if (static_cast<std::int32_t>(oldCount - 1u) >= 0)
        {
            std::uint32_t remaining = oldCount;
            do
            {
                entry -= 20;
                CSF_AT(void**, entry, 0x4) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = static_cast<std::uint32_t>(CoreMemory_Free(oldEntries - 4));
        self->field_0000 = 0;
    }

    self->field_0000 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newEntries));
    return result;
}

HOOK(0x004017A0, Table20_GrowEntryArray, "auto");
