#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall S_0048c9f0_RebuildActiveCounterMask(
    S_0048c9f0* self, void* /*edx*/)
{
    CSF_AT(std::uint16_t, self, 0x20) = 0;

    const std::int16_t previousSecond =
        static_cast<std::int16_t>(self->field_0022);
    self->field_0022 = 0;

    const std::int16_t previousThird =
        static_cast<std::int16_t>(self->field_0024);
    self->field_0024 = 0;

    std::uint32_t remaining = self->field_0030;
    if (remaining != 0)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(self->field_0028);
        do
        {
            if ((CSF_AT(std::uint8_t, entry, 0x4) & 1) != 0)
                CSF_AT(std::uint16_t, self, 0x20) +=
                    CSF_AT(std::int8_t, entry, 0x5);

            if ((CSF_AT(std::uint8_t, entry, 0x4) & 2) != 0)
                self->field_0022 += CSF_AT(std::int8_t, entry, 0x5);

            if ((CSF_AT(std::uint8_t, entry, 0x4) & 4) != 0)
                self->field_0024 += CSF_AT(std::int8_t, entry, 0x5);

            entry += 8;
            --remaining;
        } while (remaining != 0);
    }

    char mask = 0;
    if (CSF_AT(std::int16_t, self, 0x20) > 0 || previousSecond > 0)
        mask = 1;

    if (static_cast<std::int16_t>(self->field_0022) > 0 ||
        previousThird > 0)
        mask |= 2;

    // The original's other operand is an uninitialized stack word.
    if (static_cast<std::int16_t>(self->field_0024) > 0)
        mask |= 4;

    return mask;
}

HOOK_KEEP_ECX(0x0048CCA0, S_0048c9f0_RebuildActiveCounterMask, 0, "auto");
