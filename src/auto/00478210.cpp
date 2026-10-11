#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008317c8_GetFirearmCategory(C_008317c8* self, void* /*edx*/)
{
    switch (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0040), 0x74))
    {
    case 1:
        return self->field_0070 ? 3 : 1;
    case 6:
    case 7:
    case 8:
    case 0xA:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x15:
    case 0x18:
    case 0x19:
        return 3;
    default:
        return 1;
    }
}

HOOK(0x00478210, C_008317c8_GetFirearmCategory, "auto");
