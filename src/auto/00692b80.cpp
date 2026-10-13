#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall S_00692b80_GetChildCount(S_00692b80* self, void* /*edx*/)
{
    if (self->field_000b == 0xFF)
    {
        const void* children = reinterpret_cast<const void*>(self->field_0000);
        const uint32_t secondCount = CSF_AT(uint32_t, children, 0x40);
        const uint32_t firstCount = CSF_AT(uint32_t, children, 0x10);
        return static_cast<int>(secondCount + firstCount);
    }

    switch (CSF_AT(int8_t, self, 0x0A))
    {
    case 0:
        return 1;
    case 1:
    case 2:
        return CSF_AT(int, self, 0x04);
    case 3:
    case 4:
    case 5:
    default:
        return 0;
    }
}

HOOK(0x00692B80, S_00692b80_GetChildCount, "auto");
