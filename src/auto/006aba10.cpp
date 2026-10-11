#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static uint32_t* __fastcall S_00526d40_ConvertDirection(
    S_00526d40* self, void* /*edx*/, uint32_t* output)
{
    const float z = CSF_AT(float, self, 0x8);
    const float y = CSF_AT(float, self, 0x4);
    float x;
    std::memcpy(&x, &self->field_0000, sizeof(x));

    if (x * x + y * y + z * z == g_fZero)
    {
        output[0] = self->field_0000;
        output[1] = CSF_AT(uint32_t, self, 0x4);
        output[2] = CSF_AT(uint32_t, self, 0x8);
    }
    else
    {
        uint32_t direction[3];
        direction[0] = self->field_0000;
        direction[1] = CSF_AT(uint32_t, self, 0x4);
        direction[2] = CSF_AT(uint32_t, self, 0x8);

        NormalizeVector3(direction, nullptr);

        output[0] = direction[0];
        output[1] = direction[1];
        output[2] = direction[2];
    }
    return output;
}

HOOK(0x006ABA10, S_00526d40_ConvertDirection, "auto");
