#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using ComponentPoolTable = uint32_t[256];

CSF_GLOBAL(ComponentPoolTable, g_ComponentPools, 0x008BAF48);
CSF_FUNC(int, __fastcall, CoreParser_ParseIntegerValue, 0x00692800,
         (void* self, void* edx));

static int __fastcall S_00692b80_ParseIntegerComponent(
    S_00692b80* self, void* /*edx*/)
{
    if (self->field_000b == 0xFF)
        return CoreParser_ParseIntegerValue(
            reinterpret_cast<void*>(self->field_0000), nullptr);

    while (CSF_AT(uint8_t, self, 0x0A) == 0)
    {
        if (self->field_000b == 0xFF)
        {
            self = reinterpret_cast<S_00692b80*>(
                CSF_AT(uint32_t,
                       reinterpret_cast<void*>(self->field_0000), 0x18));
        }
        else
        {
            void* pool = reinterpret_cast<void*>(
                g_ComponentPools[self->field_000b]);

            uint32_t displacement =
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self)) -
                CSF_AT(uint32_t, pool, 0x00);
            uint32_t nextIndex = displacement / 0xC + 1;

            self = reinterpret_cast<S_00692b80*>(
                CSF_AT(uint32_t, pool, 0x00) + 12 * nextIndex);

            if (CSF_AT(uint8_t, pool, 0x14) == 0)
                self->field_000b = CSF_AT(uint8_t, pool, 0x15);
        }

        if (self->field_000b == 0xFF)
            return CoreParser_ParseIntegerValue(
                reinterpret_cast<void*>(self->field_0000), nullptr);
    }

    uint8_t type = CSF_AT(uint8_t, self, 0x0A);
    if (type == 3)
        return CSF_AT(int, self, 0x04);
    if (type == 4)
        return static_cast<int>(
            static_cast<int64_t>(CSF_AT(float, self, 0x04)));

    return 0;
}

HOOK(0x00692C40, S_00692b80_ParseIntegerComponent, "auto");
