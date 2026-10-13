#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using ComponentPoolTable = uint32_t[256];
CSF_GLOBAL(ComponentPoolTable, g_ComponentPools, 0x008BAF48);

static S_00692b80* __fastcall S_00692b80_GetComponent(
    S_00692b80* self, void* /*edx*/, unsigned int index)
{
    if (self->field_000b == 0xFF)
    {
        auto* storage = reinterpret_cast<uint8_t*>(self->field_0000);
        if (index >= 0xA)
        {
            auto* overflow = CSF_AT(uint8_t*, storage, 8);
            return CSF_AT(S_00692b80*, overflow, 4 * index - 40);
        }
        return CSF_AT(S_00692b80*, storage, 4 * index + 24);
    }

    auto* pool = reinterpret_cast<uint8_t*>(
        g_ComponentPools[self->field_000b]);
    const uint32_t base = CSF_AT(uint32_t, pool, 0);
    const uint32_t offset = reinterpret_cast<uint32_t>(self) - base;

    auto* component = reinterpret_cast<S_00692b80*>(
        base + 12 * (offset / 0xC + 1));

    for (unsigned int remaining = index; remaining; )
    {
        --remaining;
        component = reinterpret_cast<S_00692b80*>(
            base + 12 * component->field_0000);
    }

    if (!CSF_AT(uint8_t, pool, 0x14))
        component->field_000b = CSF_AT(uint8_t, pool, 0x15);

    return component;
}

HOOK(0x00692BD0, S_00692b80_GetComponent, "auto");
