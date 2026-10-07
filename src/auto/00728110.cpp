#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, AppendEntry, 0x00727F20,
         (void* self, void* edx, std::uint32_t* value));

static std::uint32_t __fastcall C_008655e8_AddUniqueEntry(
    C_008655e8* self, void* /*edx*/, std::uint32_t value)
{
    const std::uint32_t begin = self->field_001c;
    std::uint32_t count = 0;
    if (begin != 0) {
        const std::uint32_t byteDistance = self->field_0020 - begin;
        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDistance) >> 2);
    }

    std::uint32_t index = 0;
    if (count != 0) {
        const auto* entry =
            reinterpret_cast<const std::uint32_t*>(self->field_001c);
        do {
            if (*entry == value)
                break;
            ++index;
            ++entry;
        } while (index < count);
    }

    if (index == count) {
        return AppendEntry(
            reinterpret_cast<std::uint8_t*>(self) + 0x18,
            nullptr, &value);
    }
    return count;
}

HOOK(0x00728110, C_008655e8_AddUniqueEntry, "auto");
