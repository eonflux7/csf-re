#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_SetPosition(C_008670f0* self, void* /*edx*/, int* position)
{
    uint32_t result = self->field_0098;
    const uint32_t oldY = self->field_009c;

    if (static_cast<uint32_t>(position[0]) != result ||
        static_cast<uint32_t>(position[1]) != oldY)
    {
        self->field_0010 = 1;
        const uint32_t width = self->field_00a0 - self->field_0098;
        const uint32_t height = self->field_00a4 - self->field_009c;

        const uint32_t newX = static_cast<uint32_t>(position[0]);
        self->field_0098 = newX;
        self->field_009c = static_cast<uint32_t>(position[1]);

        const uint32_t right = newX + width;
        result = self->field_009c + height;
        self->field_00a0 = right;
        self->field_00a4 = result;
    }

    return static_cast<int>(result);
}

HOOK(0x00732E90, C_008670f0_SetPosition, "auto");
