#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the signed 32-bit field at +0x44 as a double, optionally scaled by the
// float factor at +0x54 when the flag argument is non-zero.
static double __fastcall C_00867444_GetScaledField0044(C_00867444* self, void* /*edx*/, char a2)
{
    double result = static_cast<double>(static_cast<int32_t>(self->field_0044));
    if (a2)
        result *= self->field_0054;
    return result;
}

HOOK(0x00739460, C_00867444_GetScaledField0044, "auto");
