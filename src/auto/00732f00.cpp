#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_SetSize(
    C_008670f0* self, void* /*edx*/, uint32_t* size)
{
    const uint32_t height = self->field_00a4 - self->field_009c;
    uint32_t result = self->field_00a0 - self->field_0098;

    if (size[0] != result || size[1] != height)
    {
        self->field_0010 = 1;
        self->field_00d8 = 1;

        result = size[0] + self->field_0098;
        const uint32_t top = self->field_009c;
        self->field_00a0 = result;
        self->field_00a4 = size[1] + top;
    }

    return static_cast<int>(result);
}

HOOK(0x00732F00, C_008670f0_SetSize, "auto");
