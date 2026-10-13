#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Returns the current animation time stored in the animation object reached through
// this->field_0004 -> +0x20, or 0.0 when that object or its active flag is missing.
static double __fastcall C_00863430_GetCurrentAnimTime(C_00863430* self, void* /*edx*/, int /*a2*/)
{
    double result = g_fZero;

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0004);
    uint8_t* animState = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x20));
    if (animState != nullptr)
    {
        if (CSF_AT(uint32_t, animState, 0) != 0)
            result = CSF_AT(float, animState, 4);
    }
    return result;
}

HOOK(0x00708360, C_00863430_GetCurrentAnimTime, "auto");
