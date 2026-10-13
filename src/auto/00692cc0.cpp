#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, CoreParser_ReadNumericField, 0x00692880, (int* self, void*));

using ComponentPoolEntries = std::uint32_t[256];
CSF_GLOBAL(ComponentPoolEntries, g_ComponentPools, 0x008BAF48);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __fastcall S_00692b80_ReadComponentFloat(
    S_00692b80* self, void* /*edx*/)
{
    if (self->field_000b == 0xFF)
        return CoreParser_ReadNumericField(
            reinterpret_cast<int*>(self->field_0000), nullptr);

    while (CSF_AT(std::uint8_t, self, 0xA) == 0)
    {
        if (self->field_000b == 0xFF)
        {
            self = CSF_AT(S_00692b80*,
                reinterpret_cast<void*>(self->field_0000), 0x18);
        }
        else
        {
            auto* pool = reinterpret_cast<void*>(
                g_ComponentPools[self->field_000b]);

            const std::uint32_t offset =
                static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(self)) -
                CSF_AT(std::uint32_t, pool, 0);

            self = reinterpret_cast<S_00692b80*>(
                CSF_AT(std::uint32_t, pool, 0) +
                12 * (offset / 0xC + 1));

            if (CSF_AT(std::uint8_t, pool, 0x14) == 0)
                self->field_000b = CSF_AT(std::uint8_t, pool, 0x15);
        }

        if (self->field_000b == 0xFF)
            return CoreParser_ReadNumericField(
                reinterpret_cast<int*>(self->field_0000), nullptr);
    }

    const std::uint8_t type = CSF_AT(std::uint8_t, self, 0xA);
    if (type == 3)
        return static_cast<double>(CSF_AT(std::int32_t, self, 4));
    if (type == 4)
        return CSF_AT(float, self, 4);
    return g_fZero;
}

HOOK(0x00692CC0, S_00692b80_ReadComponentFloat, "auto");
