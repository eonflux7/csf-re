#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00864508_ContainsEntry(
    C_00864508* self, void* /*edx*/, int entryValue, char allowVirtualLookup)
{
    if (entryValue == 0)
        return 0;

    const std::uint32_t beginAddress = self->field_0048;
    std::uint32_t entryCount = 0;
    if (beginAddress != 0) {
        const std::uint32_t byteDifference = self->field_004c - beginAddress;
        entryCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    if (entryCount != 0) {
        const auto* entry =
            reinterpret_cast<const std::uint32_t*>(self->field_0048);
        std::uint32_t index = 0;
        do {
            if (*entry == static_cast<std::uint32_t>(entryValue))
                return 1;
            ++index;
            ++entry;
        } while (index < entryCount);
    }

    if (allowVirtualLookup != 0) {
        using LookupMethod =
            std::uint8_t (__fastcall*)(C_00864508*, void*, int);
        if (reinterpret_cast<LookupMethod>(self->vtbl[0x78 / 4])(
                self, nullptr, entryValue) != 0)
            return 1;
    }

    return 0;
}

HOOK(0x0071A8E0, C_00864508_ContainsEntry, "auto");
