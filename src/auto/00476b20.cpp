#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008319c0_GetRecordScalarValue(
    C_008319c0* self, void* /*edx*/)
{
    if (!self->vSlot9C())
        return 1;

    const auto* record = reinterpret_cast<const uint8_t*>(self->field_0040);
    switch (CSF_AT(uint32_t, record, 0xD4))
    {
    case 0:
        return static_cast<int>(self->field_001c);
    case 1:
        return static_cast<int>(self->field_001c + self->field_0024);
    case 2:
        return static_cast<int>(
            self->field_001c +
            self->field_0020 * CSF_AT(uint32_t, record, 0x104));
    default:
        return 0;
    }
}

HOOK(0x00476B20, C_008319c0_GetRecordScalarValue, "auto");
