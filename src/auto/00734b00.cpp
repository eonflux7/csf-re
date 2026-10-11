#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_008670f0_GetDimensions(
    C_008670f0* self, void* /*edx*/, uint32_t* dimensions)
{
    const uint32_t bottom = self->field_00a4;
    const uint32_t left = self->field_0098;
    const uint32_t height = bottom - self->field_009c;
    const uint32_t width = self->field_00a0 - left;
    dimensions[0] = width;
    dimensions[1] = height;
    return dimensions;
}

HOOK(0x00734B00, C_008670f0_GetDimensions, "auto");
