#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0,
               (void* self, void* edx, int growth));

static std::uint32_t __fastcall C_008453a4_AddUniqueEntry(
    C_008453a4* self, void* /*edx*/, std::uint32_t entry)
{
    const std::uint32_t count = self->field_0010;
    std::uint32_t result = 0;

    if (count != 0) {
        auto* current = reinterpret_cast<std::uint32_t*>(self->field_0008);
        while (*current != entry) {
            ++result;
            ++current;
            if (result >= count)
                goto Append;
        }

        if (result != UINT32_MAX)
            return result;
    }

Append:
    if (self->field_0010 >= self->field_000c) {
        GrowButtonArray(
            reinterpret_cast<std::uint8_t*>(self) + 0x08,
            nullptr,
            CSF_AT(int, self, 0x14));
    }

    result = self->field_0010;
    auto* destination =
        reinterpret_cast<std::uint32_t*>(self->field_0008) + result;
    ++result;
    self->field_0010 = result;
    *destination = entry;
    self->field_0018 = 1;
    return result;
}

HOOK(0x0055BF60, C_008453a4_AddUniqueEntry, "auto");
