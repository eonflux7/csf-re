#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall S_00749b70_SetEntryValue(
    S_00749b70* self, void* /*edx*/, int index, int value)
{
    switch (index)
    {
    case 0:
        self->field_0004 = value;
        return value;
    case 1:
        CSF_AT(uint32_t, self, 0x08) = value;
        return index;
    case 2:
        CSF_AT(uint32_t, self, 0x0C) = value;
        return value;
    case 3:
        CSF_AT(uint32_t, self, 0x10) = value;
        return index;
    case 4:
        CSF_AT(uint32_t, self, 0x14) = value;
        return value;
    case 5:
        CSF_AT(uint32_t, self, 0x18) = value;
        return index;
    case 6:
        CSF_AT(uint32_t, self, 0x1C) = value;
        return value;
    case 7:
        CSF_AT(uint32_t, self, 0x20) = value;
        return index;
    case 8:
        CSF_AT(uint32_t, self, 0x24) = value;
        return value;
    case 9:
        CSF_AT(uint32_t, self, 0x28) = value;
        return index;
    case 10:
        CSF_AT(uint32_t, self, 0x2C) = value;
        return value;
    case 11:
        CSF_AT(uint32_t, self, 0x30) = value;
        return index;
    case 12:
        CSF_AT(uint32_t, self, 0x34) = value;
        return value;
    case 13:
        CSF_AT(uint32_t, self, 0x38) = value;
        return index;
    case 14:
        CSF_AT(uint32_t, self, 0x3C) = value;
        return value;
    case 15:
        CSF_AT(uint32_t, self, 0x40) = value;
        return index;
    default:
        return index;
    }
}

HOOK(0x00749C70, S_00749b70_SetEntryValue, "auto");
