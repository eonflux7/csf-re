#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_008670f0_GetValuePair(
    C_008670f0* self, void* /*edx*/, uint32_t* output)
{
    const uint32_t second = self->field_009c;
    const uint32_t first = self->field_0098;
    output[0] = first;
    output[1] = second;
    return output;
}

HOOK(0x00734AE0, C_008670f0_GetValuePair, "auto");
